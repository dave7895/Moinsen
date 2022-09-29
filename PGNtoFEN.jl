using Chess
using Chess.PGN

infilename = "doneGames.pgn"
infile2name = "slurmGames.pgn"

outfilename = "positions.epd"
outfilename2 = "newpositions.epd"

infile = open(infilename)

reader = PGNReader(infile)

function numerical_result(g)
   result::String = g.headers.result
   if length(result) == 3
      if result[1] == '1'
         return 1
      else
         return 0
      end
   end
   return 0.5
end

# TODO filter if termination == abandoned

#=
game = readgame(reader, annotations=true)
=#

function fensandresults(reader)
   gameposs = []

   while gotonextgame!(reader)
      game = readgame(reader, annotations=true)
      skip_game = false
      for header in game.headers.othertags
         if header.name == "Termination"
            skip_game = true
            break
         end
      end
      skip_game && continue
      while !isatend(game)
         forward!(game)
         n = game.node
         d = n.data
         b = board(game)
         if ischeck(b)
            continue
         end
         if !isatend(game) && pieceon(b, to(nextmove(game))) != EMPTY
            continue
         end
         if haskey(d, "comment")
            com = d["comment"]
            evaluation = split(com, '/')[1]
            if isletter(evaluation[2])
               continue
            end
            numEval = tryparse(Float64, evaluation)
            if isnothing(numEval)
               println(evaluation)
               println(com)
               println(game)
            elseif abs(numEval) > 2000
               continue
            end
         end
         push!(gameposs, (numerical_result(game), fen(b)))
      end
   end
   return gameposs
end

println("starting reading of local games")
@time gameposs = fensandresults(reader)

println("putting fens into single string")
@time arrstring = join([join(game, " ") for game in gameposs], "\n")

println("writing local fens to file")
@time open(outfilename, "w") do io
   println(io, arrstring)
end

println("processing SLURM file")
function appendToOutfile(infilename)
   @time open(infilename, "r") do inio
      reader = PGNReader(inio)
      @time gameposs = fensandresults(reader)
      arrstring = join([join(game, " ") for game in gameposs], "\n")
      open(outfilename, "a") do outio
         println(outio, arrstring)
      end
   end
end

appendToOutfile(infile2name)
appendToOutfile("slurmGames2.pgn")
#appendToOutfile("slurmGamesNew.pgn")
appendToOutfile("pstGames.pgn")
