from os import system
from sys import argv

RECOMPILE = False

if len(argv) > 1 and argv[1] == "recompile":
    RECOMPILE = True

commands = ["./lexer"]

recompile_commands = {"./lexer": 'gcc lexer.c "../src/lexer.c" "../src/token.c"'}

for command in commands:
    if RECOMPILE:
        recom = recompile_commands.get(command)
        print("Recompiling {}: {}".format(command, recom))
        system(recom)

    print("Running '%s'" % command)
    system(command)
