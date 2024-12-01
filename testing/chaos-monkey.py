import sys
import os

def spawn(nb):
    for x in range(nb):
        os.system("docker run -d chaos:dev")



if len(sys.argv) > 1:
    if sys.argv[1] == "--build":
        os.system("docker build -t chaos:dev -f ./dockerfile .")
    elif sys.argv[1] == "--nb":
        if len(sys.argv) > 2:
            spawn(sys.argv[2])
    elif sys.argv[1] == "--help":
        print("--build : permet le build de l'image docker pour le chaos-monkey")
        print("--nb : prends un arguments x = le nombre de container à spawn")
    else:
        print("arguments pas reconu....")
        print("--build -> permet de build l'image docker")
        print("--nb -> permet de définir de le nombre de container qui va être spawn pour le test")
else:
    spawn(10)