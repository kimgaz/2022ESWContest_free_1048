class Protocol:
    def __init__(self):
        self.eval = Seperate()

    def LIST_UPDATE(self,list_sheet):
        self.user_list = list_sheet

    def B2I(self,num):
        return int(num.hex(),16)

    def OK(self,permission):
        if(permission == 1):
             pass
        else:
            raise Exception("Protocol disagree")
    
    def send_protocol(self,client):
        print("SEND_PROTOCOL")
        client.send(bytes([255]))
        client.send(bytes([0]))

        ans = client.recv(1)
        ans = self.B2I(ans)

        if(ans == 1):
            return 1
        else:
            return 0

    def hear_protocol(self,client):
        print("HEAR_PROTOCOL")
        A = client.recv(1)
        B = client.recv(1)
        A = self.B2I(A)
        B = self.B2I(B)
        self.From = client

        ans = (A & ~B)

        if(ans == 255):
            client.send(bytes([1]))#프로토콜이 맞으면 1을 전송
            return 1
            
        else:
            client.send(bytes([0]))#프로토콜이 틀리면 0을 전송
            return 0
     
    def classifier(self,client):
        head = client.recv(1)
        head = self.B2I(head)
        response = head >> 7
        func = (head & 127)

        return response,func
    
    def control(self,response,func,client):
        print("Contorlling...")

        if(func == 1):
            return self.eval.Start(response,self.user_list)

        elif(func ==2):
            return self.eval.Come(response,self.user_list,client)

        elif(func == 3):
            return self.eval.Quantity(response)

        elif(func == 4):
            return self.eval.Arrive(response,client)
        
        elif(func == 5):
            return self.eval.Negative(response)

        elif(func == 6):
            return self.eval.Positive(response)

        elif(func == 7):
            return self.eval.Done(response)

        else:
            pass
            

class Seperate:
    #Func 모음
    def __init__(self):
        self.Come_list={}

    def Start(self,response,data):#0bX000 0001
        print("START!")
        if(response):
            if 'SCARA1' in data:
                return bytes([1])#최소 충족 인원 만족
            else:
                return bytes([0])#최소 충족 인원 불만족
        else:
            pass

    def Come(self,response,data,client):#0bX000 0002
        print("COME!")
        if(response):
            AGVS = []
            for key,val in data.items():
                if "AGV" in key:
                    AGVS.append(val)
                    #AGV리스트 확인

            for i in AGVS:
                i.send(bytes([130])) #0b1000 0010
                ans = int(i.recv(1),16)
                if(ans == 1):
                    self.Come_list[i]=client
                    return bytes([1]) #갈 수 있는 가능인원 있음

            return bytes([0])#갈 수 있는 인원이 없음
        else:
            pass
    
    def Quantity(self,response):
        if(response):
            pass
        else:
            pass

    def Arrive(self,response,client):
        if(response):
            pass

        else:
            #We have only one AGV, So if you want to operate more AGV, Modify here
            scara = self.Come_list[client]
            
            scara.send(bytes([255]))
            scara.send(bytes([0]))

            ans = int(scara.recv(1).hex(),16)
            if(ans != 1):
                raise Exception("Wrong Protocol")

            scara.send(bytes([0x01]))#AGV is arrived, So Start taking box
            del scara[client]

    def Negative(self,response):
        if(response):
            pass
        else:
            pass

    def Positive(self,response):
        if(response):
            pass
        else:
            pass

    def Done(self,response):
        if(response):
            pass

        else:
            pass