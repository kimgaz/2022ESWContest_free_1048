import socket
import threading

from protocol import Protocol

class Server:
    def __init__(self):
        self.prt = Protocol()
        self.ip = socket.gethostbyname(socket.gethostname())
        self.port = 80
        print("ADDRESS:",self.ip,"PORT:",self.port)
        print("server on")

        self.SK = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.SK.bind((self.ip,self.port))
        print("server start")

        self.SK.listen(5)
        print("server listening...")

        self.client_list = {'SERVER':0}
        self.INTRODUCE = bytes([255])

    def TrafficLight(self,client,user):
        print("Thread start")
        while True:
            try:
                self.prt.LIST_UPDATE(self.client_list)
                print("hearing...protocol")
                key = self.prt.hear_protocol(client)
                self.prt.OK(key)
                print("protocol correct")

                response,func = self.prt.classifier(client)
                answer = self.prt.control(response,func,client)
                self.prt.OK(key)
                client.send(answer)

            except Exception as e:
                print("Error reason:",e)
                print("Uncommon connection occur...")
                print("Name:",user)
                client.close()
                break

        del self.client_list[user]
        print(self.client_list)
        print("Thread end")

    def Accept(self):
        while True:
            client,address = self.SK.accept()
            print(address,"is in")

            client.send(self.INTRODUCE)
            try:
                name = client.recv(1024).decode()
            except:
                name = client.recv(1024)

            print(name,"이 참가했습니다")
            self.client_list[name]=client
            print(self.client_list)

            control = threading.Thread(target = self.TrafficLight,args=(client,name,),daemon=True)
            control.start()