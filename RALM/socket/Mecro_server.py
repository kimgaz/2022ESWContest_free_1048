import socket
import threading
import time as t

user_list={}

class MECRO:
    def __init__(self):
        self.ip = socket.gethostbyname(socket.gethostname())
        self.port = 80
        print("ADDRESS:",self.ip,"PORT:",self.port)
        print("server on")

        self.SK = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.SK.bind((self.ip,self.port))
        print("server start")

        self.SK.listen(10)
        print("server listening...")

        self.client_list = {'SERVER':0}
        self.INTRODUCE = bytes([255])

        self.AGV()

    def AGV(self):
        self.info={
            'SCARA1':0,
            'SCARA2':0,
            'DONE1':0,
            'DONE2':0,
            'BOX':0
        }

    def THREAD(self,client,user):
        print(user,"Thread start")
        while True:
            try:
                self.Hear(client)

            except Exception as e:
                print("Error reason:",e)
                print("Uncommon connection occur...")
                print("Name:",user)
                client.close()
                break
            
        try:
            del user_list[user]
        except:
            pass

        print(user_list)
        print("Thread end")

    def Accept(self):
        while True:
            client,address = self.SK.accept()
            print(address,"is in")

            client.send(bytes([255]))

            try:
                name = client.recv(1024).decode()
            except:
                 name = client.recv(1024)

            print(name,"이 참가했습니다")
            user_list[name] = client

            control = threading.Thread(target = self.THREAD,args=(client,name,),daemon=True)
            control.start()

    def Hear(self,client):
        while True:
            ans = int(client.recv(1).hex(),16)
            self.Split(client,ans)
            t.sleep(2)
        
    def Split(self,client,ans):
        if(ans == 1):#start Signal
            print("START SIGNAL")
            if "SCARA1" in user_list: #START
                client.send(bytes([109]))

            else:#Negative
                client.send(bytes([110]))

        elif(ans == 2):#Nothing
            pass

        elif(ans == 3):#SET AGV POSITION SCARA1
            print("SET AGV POSITION SCARA1")
            self.info['SCARA1'] = 1
            client.send(bytes([109]))
        
        elif(ans == 4):#SET AGV POSITION SCARA2
            print("SET AGV POSITION SCARA2")
            self.info['SCARA2'] = 1
            client.send(bytes([109]))

        elif(ans == 5):#Nothing
            pass

        elif(ans == 6):#GET AGV POSITION SCARA1
            print("GET AGV POSITION SCARA1")
            if(self.info['SCARA1']):
                #IF AGV IS EXSIST IN FRONT OF SCARA1
                client.send(bytes([109]))
                self.info['SCARA1'] = 0 #RESET POSITION

            else:
                #IF AGV IS NOT EXSIST IN FRONT OF SCARA1
                client.send(bytes([110]))
        
        elif(ans == 7):#GET AGV POSITION SCARA2
            print("GET AGV POSITION SCARA2")
            if(self.info['SCARA2']):
                #IF AGV IS EXSIST IN FRONT OF SCARA2
                client.send(bytes([109]))
                self.info['SCARA2'] = 0

            else:
                #IF AGV IS EXSIST IN FRONT OF SCARA2
                client.send(bytes([110]))
        
        elif(ans == 8):#Nothing
            pass

        elif(ans == 9):#SET SCARA1 DONE
            print("SET SCARA1 DONE")
            self.info['DONE1'] = 1
        
        elif(ans == 10):#SET SCARA2 DONE
            print("SET SCARA2 DONE")
            self.info['DONE2'] = 1
        
        elif(ans == 11):#Nothing
            pass

        elif(ans == 12):#GET SCARA1 DONE
            print("GET SCARA1 DONE")
            if(self.info['DONE1']==1):
                #IF SCARA1 Done His work
                print("HEY!")
                client.send(bytes([200]))
                self.info['DONE1'] = 0
            
            else:
                #IF SCARA1 not Done His work
                print("HEY")
                client.send(bytes([110]))
        
        elif(ans == 13):#GET SCARA2 DONE
            print("GET SCARA2 DONE")
            if(self.info['DONE2']==1):
                #IF SCARA2 Done His work
                client.send(bytes([109]))
                self.info['DONE2']  = 0
            
            else:
                #IF SCARA2 not Done His work
                client.send(bytes([110]))

        elif(ans == 19):#Box 0
            self.info['BOX'] = 0
            print("0!")

        elif(ans == 20):#Box 1
            self.info['BOX'] = 1
            print("1!")
        
        elif(ans == 21):#BOX 2
            self.info['BOX'] = 2
            print("2!")

        elif(ans == 22):#BOX 3
            self.info['BOX'] = 3
            print("3!")

        elif(ans == 23):#BOX 4
            self.info['BOX'] = 4
            print("4!")

        elif(ans == 30):#Box GET
            print("BOX RETURN")
            client.send(bytes(self.info['BOX']))
            
        else:
            client.send(bytes([110]))
        
        


                
if __name__ == "__main__":
    A = MECRO()
    A.Accept()