#include "Define.h"

string GetMessageFromCommand(string Command, bool TakeCarriageReturn) {
    FILE *fp;
    char path[BUF_SIZE];
    memset(path, 0, sizeof (path));
    fp = OpenPipe(fp, Command.c_str(), "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    string message = "";
    /* Read the output a line at a time - output it. */
    while (ReadPipe(path, fp, sizeof (path)) != NULL) {
        //        cout<<path<<" ";
        if (TakeCarriageReturn)path[strlen(path) - 1] = 0;
        message += path;
        message += " ";
    }
    /* close */
    ClosePipe(fp);

    return message;
}

void Login(string Username, string Password, string Email) {
    cout << "-----------LOGIN " << Username << "-----------" << endl;
    string message = "docker login -u ";
    message += Username;
    message += " -p ";
    message += Password;
    message += " -e ";
    message += Email;
    system(message.c_str());
}

void Pull(string Repository) {
    cout << "-----------PULLING " << Repository << "-----------" << endl;
    string message = "docker pull ";
    message += Repository;
    cout << "-" << message.c_str() << "-" << endl;
    system(message.c_str());
}

void DeleteImage(string ImageId) {
    cout << "-----------DELETING " << ImageId << "-----------" << endl;
    string message = "docker rmi -f ";
    message += ImageId;
    system(message.c_str());
}

string GetImageId(string Repository, bool latest) {
    cout << "-----------GETTING IMAGE FROM " << Repository << "-----------" << endl;
    /* Open the command for reading. */
    string Commande = "docker images -q ";
    Commande += Repository;
    if(latest)Commande+= ":latest";
    cout << "Commande : " << Commande << endl;
    string ImageId = GetMessageFromCommand(Commande);
    cout << "Image Id : " << ImageId << endl;
    return ImageId;
}

string GetContainerIdFromImage(string ImageId, bool IsTurning) {
    cout << "-----------GETTING CONTAINERS FROM " << ImageId << "-----------" << endl;
    string Commande = "docker ps -q ";
    if (IsTurning) Commande += "-a ";
    Commande += "--filter=ancestor=";
    Commande += ImageId;
    cout << "Get Command : " << Commande << endl;
    string ContainerId = GetMessageFromCommand(Commande);
    cout << "Container Id : " << ContainerId << endl;
    return ContainerId;
}

string GetImageIdFromContainer(string ContainerId, bool IsTurning) {
    cout << "-----------GETTING IMAGE FROM " << ContainerId << "-----------" << endl;
    string Commande = "docker ps --filter=Id=";
    Commande += ContainerId;
    Commande += " --format=""{{.Image}}"" ";
    if (IsTurning) Commande += "-a ";
    cout << "Get Command : " << Commande << endl;
    string ImageId = GetMessageFromCommand(Commande);
    cout << "Image Id : " << ImageId << endl;
    return ImageId;
}

void StopContainerId(string ContainerId) {
    cout << "-----------STOPPING CONTAINERS : " << ContainerId << "-----------" << endl;
    string message = "docker stop ";
    message += ContainerId;
    system(message.c_str());
}

void DeleteContainerId(string ContainerId) {
    cout << "-----------DELETING CONTAINERS : " << ContainerId << "-----------" << endl;
    string message = "docker rm ";
    message += ContainerId;
    system(message.c_str());
}

void DeleteAllContainers() {
    cout << "-----------DELETING ALL CONTAINERS-----------" << endl;
    string message = "docker rm ";
    string listeContainers = GetMessageFromCommand("docker ps -a -q");
    if (listeContainers.size() <= 0) {
        //pas de containers lancés
        cout << "Auncun containers de lancés" << endl;
    } else {
        int nbEspace = 0;
        for (int i = 0; i < listeContainers.size(); i++) {//on va compter le nombre d'espace pour savoir cb il y a de contenairs
            if (listeContainers[i] == ' ')nbEspace++;
        }
        cout << "Suppression de " << nbEspace << " Containers " << endl;
        message += listeContainers;
        system(message.c_str());
    }
}

void ShowContainers(bool All) {
    cout << "-----------SHOWING CONTAINERS : " << "-----------" << endl;
    string commande = "docker ps ";
    if (All)commande += "-a";
    string Message = GetMessageFromCommand(commande, false);
    cout << Message;
    write_server(Message.c_str());
}

void ShowImages() {
    cout << "-----------SHOWING IMAGES : " << "-----------" << endl;
    string commande = "docker images ";
    string Message = GetMessageFromCommand(commande, false);
    cout << Message;
    write_server(Message.c_str());
}

void StopAllContainers() {
    cout << "-----------STOPPING ALL CONTAINERS-----------" << endl;
    string message = "docker stop ";
    string listeContainers = GetMessageFromCommand("docker ps -q");
    if (listeContainers.size() <= 0) {
        //pas de containers lancés
        cout << "Auncun containers de lancés" << endl;
    } else {
        int nbEspace = 0;
        for (int i = 0; i < listeContainers.size(); i++) {//on va compter le nombre d'espace pour savoir cb il y a de contenairs
            if (listeContainers[i] == ' ')nbEspace++;
        }
        cout << "Arret de " << nbEspace << " Containers " << endl;
        message += listeContainers;
        cout << message << endl;
        system(message.c_str());
    }
}

void DeleteAllImages() {
    cout << "-----------DELETING ALL IMAGES-----------" << endl;
    system("docker rmi $(docker images -q)");
}

void RunImageId(string ImageId, bool tty, bool interractive, bool detach, bool PortForwarding, string PortEntre, string PortSortie, string Commande, bool restart) {
    cout << "-----------RUNNING IMAGE-----------" << endl;
    string message = "docker run ";
    if (tty)message += "-t ";
    if (interractive)message += "-i ";
    if (detach)message += "-d ";
    if (PortForwarding) {
        message += "-p ";
        message += PortEntre;
        message += ":";
        message += PortSortie;
        message += " ";
    }
    if (restart) message += "--restart=""always"" ";
    cout << "image :" << ImageId;
    message += ImageId;
    message += " ";
    message += Commande;
    cout << "Run commande : " << message << endl;
    system(message.c_str());
}

