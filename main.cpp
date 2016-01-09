#include "Define.h"

vector <dictionnaire> ListeDico;

void ShowDictionnary() {
    if (ListeDico.size() <= 0)cout << "Dictionnary empty" << endl;
    for (auto& i : ListeDico) {
        cout << "Conteneur : " << i.Container << " ID : " << i.ContainerIDAssocie << " Repository : " << i.Repository << " Image Id : " << i.ImageIDAssocie << " Running : " << i.IsRunning << endl;
    }
}

void SetAllImageRunningFromDictionnary(bool IsRunning) {
    if (ListeDico.size() <= 0)cout << "Dictionnary empty" << endl;
    for (auto& i : ListeDico) {
        i.IsRunning = IsRunning;
    }
}

void SetContainerRunningFromDictionnary(const char* image, bool IsRunning) {
    for (std::vector<dictionnaire>::iterator it = ListeDico.begin(); it != ListeDico.end(); ++it) {
        cout << it->Repository << " : " << image << endl;
        if (strcmp(it->Repository.c_str(), image) == 0) {
            it->IsRunning = IsRunning;
        }
    }
}

void DeleteDictionnary() {
    ListeDico.clear();
}

void DeleteImageFromDictionnary(const char* image) {
    for (std::vector<dictionnaire>::iterator it = ListeDico.begin(); it != ListeDico.end(); /*pas besoin d'incrementer*/) {
        cout << it->Repository << " : " << image << endl;
        if (strcmp(it->Repository.c_str(), image) == 0) {
            it = ListeDico.erase(it);
            cout << "Delete" << endl;
        } else ++it;
    }
}

void ChargeDictionnaire() {
    ifstream fichier("Dico.bin");
    cout << "loading dictionnary" << endl;
    dictionnaire* Dico;
    //    OpenFile(fichier,"Dico.bin",(ios::openmode)(ios::in | ios::binary));
    if (!fichier.is_open()) { // si il n'est pas ouvert, c'est qu'il n'est pas créé
        fstream CreationFichier;
        OpenFile(CreationFichier, "Dico.bin", (ios::openmode)ios::out);
        Close(CreationFichier);
    }
    fichier.read((char*) Dico, sizeof (dictionnaire));
    //    while (!fichier.eof()) {
    //        ReadFile(fichier, Dico, sizeof (dictionnaire));
    //        if(!fichier.eof())cout<<Dico->Container;//ListeDico.push_back(*Dico);
    //    }
    //    Close(fichier);
    ShowDictionnary();
}

void SaveDictionnaire() {
    fstream fichier;
    cout << "Saving dictionnary" << endl;
    OpenFile(fichier, "Dico.bin", (ios::openmode)(ios::out | ios::binary));
    for (auto i : ListeDico) {
        WriteFile(fichier, &i, sizeof (dictionnaire));
    }
    Close(fichier);
}

int main(int argc, char** argv) {
    init_connection();
    //    SetUnblocked();
    Connection("192.168.1.189");
    bool fin = false;
    string messageRecu = "";
    string Adresse = GetMacAddr();
    cout << "addr : " << Adresse << endl;
    InitTimer();
    DemarrageTimer();
    Login("thimoti26", "titi26011994", "thimoty26@gmail.com");
    StopAllContainers();
    DeleteAllContainers();
    //    ChargeDictionnaire();
    cout << "DONE" << endl;
    while (!fin) {
        messageRecu = read_server();
        if (messageRecu == "") {//serveur deco
            cout << "Serveur deconnecte, tentative de reconnection " << endl;
            init_connection();
            Connection("192.168.1.189");
            cout << "Reconnection reussi" << endl;
        } else {
            cout << messageRecu << endl;
            if (strcmp(messageRecu.c_str(), "deleteallcontainers") == 0) {
                StopAllContainers();
                DeleteAllContainers();
                DeleteDictionnary();
                cout << "DONE" << endl;
            } else {
                if (strcmp(messageRecu.c_str(), "stopallcontainers") == 0) {
                    StopAllContainers();
                    SetAllImageRunningFromDictionnary(false);
                    cout << "DONE" << endl;
                } else {
                    if (strcmp(messageRecu.c_str(), "showdictionnary") == 0) {
                        ShowDictionnary();
                        cout << "DONE" << endl;
                    } else {
                        if (strcmp(messageRecu.c_str(), "showcontainers") == 0) {
                            ShowContainers(true);
                            cout << "DONE" << endl;
                        } else {
                            if (strcmp(messageRecu.c_str(), "showimages") == 0) {
                                ShowImages();
                                cout << "DONE" << endl;
                            } else {
                                if (strcmp(messageRecu.c_str(), "end") == 0) {
                                    SaveDictionnaire();
                                    fin = true;
                                    cout << "DONE" << endl;
                                } else { //commande Complexe
                                    string ContenerChoisis = "";
                                    string Repository = "";

                                    bool tty = false;
                                    bool interractive = false;
                                    bool detach = false;
                                    bool PortForwarding = false;
                                    bool latest = true; //si il n'y a rien apres le repository, on utilise la dernere version
                                    string PortEntre = "";
                                    string PortSortie = "";
                                    string Commande = "";
                                    int i = 0;
                                    while (i < messageRecu.size() and messageRecu[i] != ':') {
                                        ContenerChoisis += messageRecu[i];
                                        i++;
                                    }
                                    if (i >= messageRecu.size()) {//c'est qu'il n'y avais pas de ':'
                                        cout << "Erreur Format" << endl;
                                        cout << "DONE" << endl;
                                    } else {
                                        i++; //consomme le ':'
                                        while (i < messageRecu.size() and messageRecu[i] != ' ') {//tant qu'on ne voit pas un espace, on continue
                                            Repository += messageRecu[i];
                                            i++;
                                            if (messageRecu[i] == ':')latest = false; //on n'utilisera pas la derniere version
                                        }
                                        cout << "----> Latest : " << latest << endl;
                                        if (strcmp(ContenerChoisis.c_str(), "deleteimage") == 0) {//ne peut etre bien fait qu'en arrétant et supprimant les containers l'utilisant
                                            StopContainerId(GetContainerIdFromImage(GetImageId(Repository, latest), true));
                                            DeleteContainerId(GetContainerIdFromImage((Repository), true));
                                            DeleteImage(GetImageId(Repository, latest));
                                            DeleteImageFromDictionnary(Repository.c_str());
                                            cout << "DONE" << endl;
                                        } else {
                                            if (strcmp(ContenerChoisis.c_str(), "stopcontainer") == 0) {//pour delete tout les containers, il faut les stopper
                                                StopContainerId(GetContainerIdFromImage(GetImageId(Repository.c_str(), latest)));
                                                SetContainerRunningFromDictionnary(Repository.c_str(), false);
                                                cout << "DONE" << endl;
                                            } else {
                                                if (strcmp(ContenerChoisis.c_str(), "deletecontainer") == 0) {
                                                    StopContainerId(GetContainerIdFromImage(GetImageId(Repository.c_str(), latest)));
                                                    DeleteContainerId(GetContainerIdFromImage(GetImageId(Repository.c_str(), latest), true));
                                                    SetContainerRunningFromDictionnary(Repository.c_str(), false);
                                                    cout << "DONE" << endl;
                                                } else {
                                                    Pull(Repository.c_str()); //si il ne l'a pas, sinon ne fait rien
                                                    //commandes
                                                    while (i < messageRecu.size() - 1) {//on regarde les commandes
                                                        if (messageRecu[i] == '-') {
                                                            i++;
                                                            if (messageRecu[i] == 't') {
                                                                tty = true;
                                                            } else {
                                                                if (messageRecu[i] == 'i') {
                                                                    interractive = true;
                                                                } else {
                                                                    if (messageRecu[i] == 'd') {
                                                                        detach = true;
                                                                    } else {
                                                                        if (messageRecu[i] == 'p' and i < messageRecu.size() - 4) { //il faut au moins 4 lettres pour le port forwarding
                                                                            PortForwarding = true;
                                                                            i++; //consomme le p
                                                                            i++; //consomme le ' '
                                                                            cout << messageRecu[i];
                                                                            cout << "-";
                                                                            while (messageRecu[i] != ' ' and i < messageRecu.size()) {
                                                                                cout << messageRecu[i];
                                                                                PortEntre += messageRecu[i];
                                                                                i++;
                                                                            }
                                                                            cout << "-";
                                                                            if (i < messageRecu.size()) { //ok
                                                                                i++; //consomme le ' '
                                                                                while (messageRecu[i] != ' ' and i < messageRecu.size()) {
                                                                                    cout << messageRecu[i];
                                                                                    PortSortie += messageRecu[i];
                                                                                    i++;
                                                                                }

                                                                            }
                                                                        } else {
                                                                            if (messageRecu[i] == 'c') {//commande, forcément le dernier
                                                                                i++; //consomme le p
                                                                                i++; //consomme le ' '
                                                                                while (i < messageRecu.size()) {
                                                                                    Commande += messageRecu[i];
                                                                                    i++;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                            }
                                                        }
                                                        i++;
                                                    }
                                                    cout << "Conteneur : " << ContenerChoisis << endl;
                                                    cout << "Repository : " << Repository << endl;
                                                    cout << "tty : " << tty << " interractive : " << interractive << " detach : " << detach << " port forwarding : " << PortForwarding << " entree : " << PortEntre << " Sortie : " << PortSortie << " Commande : " << Commande << endl;
                                                    bool ImageTrouve = false;
                                                    for (auto& i : ListeDico) {
                                                        if (i.Container == ContenerChoisis and i.ImageIDAssocie == GetImageId(Repository, latest)) {
                                                            cout << "zz ---RESTART---" << endl;
                                                            ImageTrouve = true;
                                                            cout << "Image déja dans ce container" << endl;
                                                            StopContainerId(i.ContainerIDAssocie);
                                                            DeleteContainerId(i.ContainerIDAssocie);
                                                            RunImageId(GetImageId(Repository.c_str(), latest), tty, interractive, detach, PortForwarding, PortEntre, PortSortie, Commande);
                                                            i.ContainerIDAssocie = GetContainerIdFromImage(GetImageId(i.Repository, latest), true);
                                                            i.Repository = Repository;
                                                            i.ImageIDAssocie = GetImageId(Repository, latest); //GetImageId(i.Repository);
                                                            if ((Commande.size() > 0) and interractive) {
                                                                i.IsRunning = true;
                                                                cout << "---> Container Launched " << endl;
                                                            } else {
                                                                cout << "---> Container Stopped " << endl;
                                                                i.IsRunning = false;
                                                            }
                                                        }
                                                    }
                                                    if (ImageTrouve == false) {
                                                        RunImageId(GetImageId(Repository.c_str(), latest), tty, interractive, detach, PortForwarding, PortEntre, PortSortie, Commande);
                                                        bool TrouveDansDictionnaire = false;
                                                        for (auto& i : ListeDico) {
                                                            if (i.Container == ContenerChoisis) {
                                                                cout << "!!!!!!!!!!!Changement d'image!!!!!!!!!!!" << endl;
                                                                TrouveDansDictionnaire = true;
                                                                StopContainerId(i.ContainerIDAssocie);
                                                                DeleteContainerId(i.ContainerIDAssocie);
                                                                cout << i.Repository << " => " << Repository << endl;
                                                                i.Repository = Repository;
                                                                i.ContainerIDAssocie = GetContainerIdFromImage(GetImageId(i.Repository, latest), true);
                                                                i.ImageIDAssocie = GetImageIdFromContainer(i.ContainerIDAssocie, true);
                                                                ;
                                                                if ((Commande.size() > 0) and interractive) {
                                                                    i.IsRunning = true;
                                                                    cout << "---> Container Launched " << endl;
                                                                } else {
                                                                    cout << "---> Container Stopped " << endl;
                                                                    i.IsRunning = false;
                                                                }
                                                            }
                                                        }
                                                        if (!TrouveDansDictionnaire) {
                                                            cout << "!!!!!!!!!!!Pas De Changement d'image!!!!!!!!!!!" << endl;
                                                            dictionnaire Dico;
                                                            Dico.Container = ContenerChoisis;
                                                            Dico.Repository = Repository;
                                                            Dico.ContainerIDAssocie = GetContainerIdFromImage(GetImageId(Dico.Repository, latest), true);
                                                            Dico.ImageIDAssocie = GetImageIdFromContainer(Dico.ContainerIDAssocie, true);
                                                            if ((Commande.size() > 0) and interractive) {
                                                                Dico.IsRunning = true;
                                                                cout << "---> Container Launched " << endl;
                                                            } else {
                                                                cout << "---> Container Stopped " << endl;
                                                                Dico.IsRunning = false;
                                                            }
                                                            ListeDico.push_back(Dico);
                                                        }
                                                    }
                                                    cout << "DONE" << endl;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    end_connection();
    /*
    //////////////////////////////DOCKER////////////////////////////////
    cout << "--------------STOP ALL CONTAINERS -----------------" << endl;
    StopAllContainers();
    cout << "--------------DELETE ALL CONTAINERS -----------------" << endl;
    DeleteAllContainers();
    cout << "--------------RUN TEST CONTAINER -----------------" << endl;
    RunImageId(GetImageId("thimoti26/test"), true, true, true, true, "7777", "8000", "zerobin --settings-file=/home/settings.py");
    cout << "--------------PULL DEBIAN IMAGE -----------------" << endl;
    Pull("debian");
    cout << "--------------RUN DEBIAN IMAGE -----------------" << endl;
    RunImageId(GetImageId("debian"), true, true, true, false);
    cout << "--------------RUN Google IMAGE -----------------" << endl;
    RunImageId(GetImageId("google/golang"), true, true, true, false);
    cout << "--------------STOP TEST CONTAINER -----------------" << endl;
    StopContainerId(GetContainerId(GetImageId("thimoti26/test")));
    cout << "--------------DELETE TEST CONTAINER -----------------" << endl;
    DeleteContainerId(GetContainerId(GetImageId("thimoti26/test"), true));*/
    return 0;
}

