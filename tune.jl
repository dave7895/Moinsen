using Zygote
using Chess
using SparseArrays
using LinearAlgebra
using Statistics
using Flux

startvalues = [falses(384); 100 .* [1, 3, 3, 5, 9, 0]; falses(12)]

workcopy = Float64.([startvalues; startvalues])

const jlTolibchess = [r * 8 + f for r = 7:-1:0, f = 0:7]
const phaseMultipliers = [0, 1, 1, 2, 4, 0]

struct TuneData{Ti,Tv}
   coeffs::SparseVector{Ti,Tv}
   result::Float32
end

function modifyPSTcoeff!(pstCoeffs, p::Piece, sq::Square)
   pieceMultiplier = ptype(p).val - 1
   pc = pcolor(p)
   squareAdd = pc == WHITE ? jlTolibchess[sq.val] : jlTolibchess[sq.val] ⊻ 56
   pstCoeffs[64*pieceMultiplier+squareAdd+1] += pc == WHITE ? 1 : -1
end

function getPSTcoeffs(b::Board)
   pstcoeffs = zeros(Int, 64 * 6)
   for sq in occupiedsquares(b)
      modifyPSTcoeff!(pstcoeffs, pieceon(b, sq), sq)
   end
   return pstcoeffs
end

function pieceCountVec(b)
   pCounts = zeros(Int, 6)
   for i = 1:6
      pCounts[i] = squarecount(pieces(b, WHITE, PieceType(i))) - squarecount(pieces(b, BLACK, PieceType(i)))
   end
   return pCounts
end

function attackfunction(pt::PieceType)
   if pt == PAWN
      return pawnattacks
   elseif pt == KNIGHT
      return (occ, sq) -> knightattacks(sq)
   elseif pt == BISHOP
      return bishopattacks
   elseif pt == ROOK
      return rookattacks
   elseif pt == QUEEN
      return queenattacks
   elseif pt == KING
      return (occ, sq) -> kingattacks(sq)
   end
end

function attackedbypawns(pawns::SquareSet, pc::PieceColor)
   if pc == WHITE
      return pawnshift_ne(pawns) ∪ pawnshift_nw(pawns)
   elseif pc == BLACK
      return pawnshift_se(pawns) ∪ pawnshift_sw(pawns)
   end
end

function getMoveCount(b::Board, p::Piece)
   occ = occupiedsquares(b)
   attackf = attackfunction(ptype(p))
   ownpawns = pieces(b, pcolor(p), PAWN)
   ownpawnattacks = attackedbypawns(ownpawns, pcolor(p))
   if ptype(p) == PAWN
      if pcolor(p) == WHITE
         return squarecount(ownpawnattacks ∪ pawnshift_n(ownpawns) ∪ pawnshift_n(pawnshift_n(SS_RANK_2 ∩ ownpawns)))
      else
         return squarecount(ownpawnattacks ∪ pawnshift_s(ownpawns) ∪ pawnshift_s(pawnshift_s(SS_RANK_7 ∩ ownpawns)))
      end
   end
   opppawns = pieces(b, -pcolor(p), PAWN)
   opppawnattacks = attackedbypawns(opppawns, -pcolor(p))
   possibletargets = (-occ ∪ pieces(b, coloropp(pcolor(p)))) ∩ -opppawnattacks
   ps = pieces(b, p)
   isempty(ps) && return 0
   attacked = SS_EMPTY
   for sq in ps
      attacked = attacked ∪ attacksfrom(b, sq)
   end
   squarecount(attacked ∩ possibletargets)
end


getMoveCount(b::Board, pc::PieceColor, pt::PieceType) = getMoveCount(b, Piece(pc, pt))

function moveCountVec(b::Board)
   mCounts = zeros(Int, 6)
   for i = 1:6
      mCounts[i] = getMoveCount(b, WHITE, PieceType(i)) - getMoveCount(b, BLACK, PieceType(i))
   end
   return mCounts
end

function attackedVec(b)
   aCounts = zeros(Int, 6)
   wAttack = SS_EMPTY
   for sq in pieces(b, WHITE)
      wAttack = wAttack ∪ attacksfrom(b, sq)
   end
   bAttack = SS_EMPTY
   for sq in pieces(b, BLACK)
      bAttack = bAttack ∪ attacksfrom(b, sq)
   end
   for i = 1:6
      pt = PieceType(i)
      aCounts[i] = squarecount(pieces(b, BLACK, pt) ∩ wAttack) - squarecount(pieces(b, WHITE, pt) ∩ bAttack)
   end
   return aCounts
end

function getPhase(b::Board)
   phase = 0
   for i = 1:6
      phase += squarecount(pieces(b, PieceType(i))) * phaseMultipliers[i]
   end
   return phase
end

function getBoardVector(b)
   v = [getPSTcoeffs(b); pieceCountVec(b); moveCountVec(b); attackedVec(b)]
   floatPhase = min(23, getPhase(b)) / 24
   return SparseVector{Float64,UInt16}(sparse([v .* floatPhase; v .* (1 - floatPhase)]))
end

function fillTuneData(fen::AbstractString, result::AbstractFloat)
   return TuneData(getBoardVector(fromfen(fen)), Float32(result))
end



function linearEvaluate(params::typeof(workcopy), coeffs::TuneData)
   return dot(params, coeffs.coeffs)
end

sigmoid(s, K=K) = 1 / (1 + exp(-K * s))

function error(f::AbstractFloat, E; K=K)
   (f - sigmoid(E, K))^2
end

error(params, td::TuneData, K=K) = error(td.result, linearEvaluate(params, td); K=K)

function averageError(params::typeof(workcopy), tds::AbstractVector, K=K)
   mean(td -> error(params, td, K), tds)
end

function readFile(path::AbstractString, n=Inf)
   entries = TuneData[]
   i = 0
   open(path) do f
      #while !eof(f)
      for line in eachline(f)
         i > n && break
         line = readline(f)
         isempty(line) && continue
         tokens = split(line)
         result = tryparse(Float32, tokens[1])
         isnothing(result) && continue
         b = fromfen(join(tokens[2:end], " "))
         isnothing(b) && continue
         push!(entries, TuneData(getBoardVector(b), result))
         i += 1
      end
   end
   return entries
end

function optimizeK(dataset)
   start = 0
   stop = 1
   step = 0.1
   optK = 1
   optErr = 100
   for _ = 1:10
      for locK = start:step:stop
         err = averageError(workcopy, dataset, locK)
         if err < optErr
            optErr = err
            optK = locK
         end
      end
      start = max(eps(), optK - step)
      stop = optK + step
      step *= 0.1
   end
   return optK
end

function optimizeParams(dataset, epochs=10)
   lr = 0.01
   gradnorm = 1
   #accu = zero(gradient(error, workcopy, dataset[1])[1])
   p = Flux.params(workcopy)
   opt = Flux.AdaDelta()
   lck = ReentrantLock()
   for i = 1:epochs
      gradnorm = 0
      Threads.@threads for td in dataset
         #grad = gradient(error, workcopy, td)[1]
         grad = gradient(p) do
            error(workcopy, td)
         end
         #gradnorm = max(gradnorm, maximum(grad))
         lock(lck) do
            Flux.Optimise.update!(opt, p, grad)
            #accu += grad
         end
      end
      #println(gradnorm)
      #println(maximum(accu))
      #workcopy .-= lr .* accu
      #accu = zero(accu)
      writeToFile("test.hpp", workcopy)
   end
end

#ks = 0.0001:0.000001:0.01
#[averageError(workcopy, tds, K) for K in ks]



function writeToFile(path::AbstractString, params)
   open(path, "w") do f
      writeToFile(f, params)
   end
end

function writeToFile(f, params)
   println(f, "#include <cstdint>\n#include <array>\n#include \"phasedScore.hpp\"\n")
   println(f, "// clang-format off")
   allInts = round.(Int, params)
   for offset = 0:402:402
      println(f, "constexpr std::array<int16_t, 6*64> $(offset == 0 ? "mg" : "eg")_table = {{")
      for i = 1:6*64
         print(f, lpad(allInts[i+offset], 3), ", ")
         i % 8 == 0 && println(f)
         i % 64 == 0 && println(f)
      end
      println(f, "}};\n")
   end
   println(f, "// clang-format on\n")
   t = "constexpr std::array<PhaseScore, 6> "
   names = ["pieceVal", "mobilityBonus", "attackBonus"]
   offset = 402
   for i = 1:3
      println(f, t, names[i], " = {{")
      for j = 385:390
         zeroind = j + (i - 1) * 6
         print(f, "S(", allInts[zeroind], ", ", allInts[zeroind+offset], "), ")
      end
      println(f, "}};\n")
   end
end

function loss(x::TuneData)
   return error(workcopy, x)
end

function loss(tds::Vector{TuneData})
   return mean(loss.(tds))
end

function ploss(tds::Vector{TuneData})
   accu = Threads.Atomic{Float64}(0)
   Threads.@threads for td in tds
      accu[] += error(workcopy, td)
   end
   return accu[] / length(tds)
end

println(Threads.nthreads())
@show get(ENV, "JULIA_NUM_THREADS", "")

@time dataset = readFile("positions.epd", 100000);
@time const K = optimizeK(dataset)
#@time optimizeParams(dataset)

opt = Flux.AdaGrad()
pars = Flux.params(workcopy)

data = Flux.DataLoader(dataset, batchsize=64, shuffle=true);

#gradient(loss, [dataset[1]])
for i = 1:100
   @info "Epoch $i"
   @time Flux.Optimise.train!(loss, pars, data, opt; cb=Flux.throttle(() ->
         writeToFile("test.hpp", workcopy), 10))
end

#=for bs = [1, 16, 32, 64, 128, 256]
   ds = Flux.DataLoader(dataset, batchsize=bs, shuffle=true)
   @show bs
   @time Flux.Optimise.@epochs 10 Flux.Optimise.train!(loss, pars, data, opt; cb=Flux.throttle(() ->
         writeToFile("test.hpp", workcopy), 10))
end=#
