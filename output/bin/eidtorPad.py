import tkinter as tk
from tkinter import scrolledtext, ttk, filedialog, messagebox
import socket
import os
import time
import threading
import json


def getClassType():
    with open("res/datalist/classType.json", "r", encoding="utf-8") as file:
        try:
            typeData = json.load(file)
        except json.JSONDecodeError:
            root.showLog("initError")
        file.close()
        return typeData


class client:
    def __init__(self) -> None:
        self.port = 9000
        self.host = "127.0.0.1"
        self.bRnuCl = True
        # self.tryconnect()

    def tryconnect(self, root_):
        self.root = root_
        self.isConnected = False
        self.sockCl = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while self.bRnuCl:
            try:
                self.sockCl.connect((self.host, self.port))
                root.showLog("server connected")
                print("server connected")
                self.isConnected = True
                return
            except ConnectionRefusedError:
                root.showLog("retry connect")
                print("retry connect")
                time.sleep(0.5)

    def sendMessage(self, message: str):
        if self.isConnected == True:
            try:
                self.sockCl.sendall(message.encode("utf-8"))
            except (ConnectionResetError, OSError):
                self.sockCl.close()
                self.sockCl = None
                self.tryconnect(self.root)
                return
            # data=self.sockCl.recv(1024)
            # root.showLog(data.decode('utf-8'))

    def getMessage(self):
        data = self.sockCl.recv(1024)
        root.showLog(data.decode("utf-8"))

    def closeSocket(self):
        self.sockCl.close()


class pageWithScollBar:
    def __init__(self, page_) -> None:
        self.canvas = tk.Canvas(page_)
        self.canvas.pack(side="top", expand=True, fill="both")

        self.page = tk.Frame(self.canvas)
        self.page.pack(expand=True, fill="both")

        self.canvas.create_window((0, 0), window=self.page, anchor=tk.NW)

        def setCanvas(x):
            self.canvas.config(scrollregion=self.canvas.bbox("all"))

        self.page.bind("<Configure>", setCanvas)
        self.scollBarY = ttk.Scrollbar(
            self.canvas, orient="vertical", command=self.canvas.yview
        )
        self.scollBarY.pack(side="right", fill="both")
        self.canvas.config(yscrollcommand=self.scollBarY.set)
        self.scollBarX = ttk.Scrollbar(
            self.canvas, orient="horizontal", command=self.canvas.xview
        )
        self.scollBarX.pack(side="bottom", fill="both")
        self.canvas.config(xscrollcommand=self.scollBarX.set)


#####################################################################################################
#####################################################################################################
#####################################################################################################
class mainPad:
    def __init__(self) -> None:
        self.savename = ""
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW",lambda:root.showLog('close abandoned'))
        self.root.title("editor")
        self.root.geometry("400x800+1420+100")
        self.menupage = tk.Frame(self.root, height=10, borderwidth=2, relief="groove")
        self.menupage.pack(anchor=tk.W, fill="x")
        menuBtns = {
          
            "copy": tk.Button(self.menupage, text="复制"),
            "paste": tk.Button(self.menupage, text="粘贴"),
            "delete": tk.Button(self.menupage, text="删除"),
        }
        btnsSet = {
            
            "copy": (2,1),
            "paste": (2,2),
            "delete": (2,3),
        }
        for k, v in menuBtns.items():
            v.grid(row=btnsSet[k][0], column=btnsSet[k][1])
        
        menuBtns["copy"].config(command=self.copyBtnCbk)
        menuBtns["paste"].config(command=self.pasteBtnCbk)
        menuBtns["delete"].config(command=self.deleteBtnCbk)

        notebook = ttk.Notebook(self.root)
        notebook.pack(expand=True, fill="both")
        self.totalPage = tk.Frame(self.root, height=100)
        self.totalPage.pack(fill="x")
        self._logName = tk.Label(self.totalPage, text="日志").pack(anchor=tk.W)
        self._log = scrolledtext.ScrolledText(
            self.totalPage, height=10, width=60, state="normal"
        )
        self._log.pack(fill="x")

        page1 = tk.Frame(notebook)
        page2 = tk.Frame(notebook)
        page3 = tk.Frame(notebook)

        game = pageWithScollBar(page1)
        actor = pageWithScollBar(page2)
        land = pageWithScollBar(page3)
        self.gamepage = game.page
        self.actorpage = actor.page
        self.landpage = land.page

        notebook.add(page1, text="游戏模式")
        notebook.add(page2, text="actor模式")
        notebook.add(page3, text="land模式")
        notebook.bind("<<NotebookTabChanged>>", self.clickNoteBook)

    def clickNoteBook(self, event):
        a = event.widget.select()
        c = event.widget.tab(a, "text")
        if c == "游戏模式":
            cl.sendMessage("setmode 2")
        elif c == "actor模式":
            cl.sendMessage("setmode 0")
        else:
            cl.sendMessage("setmode 1")

    def showLog(self, message: str):
        self._log.insert(tk.END, message + "\n")
        self._log.yview(tk.END)

    def mainLoop(self):
        self.root.mainloop()

    def saveBtnCbk(self):
        if self.savename == "":
            self.savename = filedialog.asksaveasfilename(filetypes=[("json", "*.json")])
            if self.savename == "":
                return
        name, ext = os.path.splitext(self.savename)
        if not ext:
            self.savename = self.savename + ".json"
        self.root.title(self.savename)
        cl.sendMessage("save " + self.savename)

    def openBtnCbk(self):
        self.savename = filedialog.askopenfilename(filetypes=[("json", "*.json")])
        if self.savename == "":
            return
        self.root.title(self.savename)
        cl.sendMessage("open " + self.savename)

    def newFileBtnCbk(self):
        self.savename = ""
        self.root.title("未保存")
        cl.sendMessage("newWorld")

    def copyBtnCbk(self):
        cl.sendMessage("copy")

    def pasteBtnCbk(self):
        cl.sendMessage("paste")

    def deleteBtnCbk(self):
        cl.sendMessage("delete")


cl = client()
root = mainPad()

class menuClass:
    def __init__(self,root_:tk.Tk) -> None:
        self.menus=tk.Menu(root_,tearoff=False)
        root_.config(menu=self.menus)
        self.menuFile=tk.Menu(self.menus)
        self.menuFile.add_command(label="打开",command=root.openBtnCbk)
        self.menuFile.add_command(label="保存",command=root.saveBtnCbk)
        self.menuFile.add_command(label="新建",command=root.newFileBtnCbk)
        self.menus.add_cascade(label="文件",menu=self.menuFile)
menus=menuClass(root.root)
# 设置位置
class padSetPos:
    def __init__(self, page_: tk.Frame) -> None:
        self.childPage = tk.Frame(page_, borderwidth=2, relief="groove")
        self.childPage.pack(anchor=tk.W)
        self.content = {
            "title": (tk.Label(self.childPage, text="位置"), (1, 2, 2), None),
            "l1": (
                tk.Label(self.childPage, text="x", width=4, anchor=tk.E),
                (2, 1, 1),
                None,
            ),
            "input1": (tk.Entry(self.childPage), (2, 2, 1), tk.StringVar()),
            "l2": (
                tk.Label(self.childPage, text="y", width=4, anchor=tk.E),
                (3, 1, 1),
                None,
            ),
            "input2": (tk.Entry(self.childPage), (3, 2, 1), tk.StringVar()),
            "l3": (
                tk.Label(self.childPage, text="z", width=4, anchor=tk.E),
                (4, 1, 1),
                None,
            ),
            "input3": (tk.Entry(self.childPage), (4, 2, 1), tk.StringVar()),
            "c": (tk.Button(self.childPage, text="设置"), (5, 2, 2), None),
        }
        for eleK, eleV in self.content.items():
            eleV[0].grid(
                row=eleV[1][0],
                column=eleV[1][1],
                sticky=tk.W,
                columnspan=eleV[1][2],
            )
            if eleV[2] != None:
                eleV[0].config(textvariable=eleV[2])

        def buttonCbk():
            text = [
                self.content["input1"][2].get(),
                self.content["input2"][2].get(),
                self.content["input3"][2].get(),
            ]
            try:
                message = f"set {float(text[0])} {float(text[1])} {float(text[2])}"
            except ValueError:
                root.showLog("should input number")
                return
            cl.sendMessage(message)

        self.content["c"][0].config(command=buttonCbk)


# 创建对象
class padCreateObj:
    def __init__(self, page_: tk.Frame) -> None:
        self.childPage = tk.Frame(page_, width=16)
        self.childPage.pack()
        self.classType = getClassType()
        types = list(self.classType["classType"].keys())
        entWidth = 8
        self.content = {
            "title": (
                tk.Label(self.childPage, text="创建对象"),
                (0, 1, 2, tk.W),
                None,
            ),  # (row column columnspan sticky),variable
            "lX": (tk.Label(self.childPage, text="X"), (1, 2, 1, tk.W), None),
            "lY": (tk.Label(self.childPage, text="Y"), (1, 3, 1, tk.W), None),
            "lZ": (tk.Label(self.childPage, text="Z"), (1, 4, 1, tk.W), None),
            "lSize": (tk.Label(self.childPage, text="尺寸"), (2, 1, 1, tk.W), None),
            "entX": (
                tk.Entry(self.childPage, width=entWidth),
                (2, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "entY": (
                tk.Entry(self.childPage, width=entWidth),
                (2, 3, 1, tk.W),
                tk.StringVar(),
            ),
            "entZ": (
                tk.Entry(self.childPage, width=entWidth),
                (2, 4, 1, tk.W),
                tk.StringVar(),
            ),
            "l1": (tk.Label(self.childPage, text="图ID"), (3, 1, 1, tk.W), None),
            "fileid": (
                tk.Entry(self.childPage, width=entWidth),
                (3, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "l2": (tk.Label(self.childPage, text="序号"), (4, 1, 1, tk.W), None),
            "picid": (
                tk.Entry(self.childPage, width=entWidth),
                (4, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "l3": (tk.Label(self.childPage, text="数量"), (6, 1, 1, tk.W), None),
            "times": (
                tk.Entry(self.childPage, width=entWidth),
                (6, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "lClassType": (
                tk.Label(self.childPage, text="类型"),
                (7, 1, 1, tk.W),
                None,
            ),
            "cboxClassType": (
                ttk.Combobox(self.childPage, values=types, state="readonly"),
                (7, 2, 3, tk.W),
                tk.StringVar(),
            ),
            "btnCreate": (tk.Button(self.childPage, text="创建",width=8), (8, 2, 1, tk.W), None),
            "btnSet":(tk.Button(self.childPage, text="设置",width=8), (8, 3, 1, tk.W), None)
        }

        for v in self.content.values():
            v[0].grid(row=v[1][0], column=v[1][1], sticky=v[1][3], columnspan=v[1][2])
            if v[2] != None:
                v[0].config(textvariable=v[2])
        self.content["btnCreate"][0].config(command=self.buttonCbk)
        self.content["btnSet"][0].config(command=self.btnSetCbk)
        self.content["cboxClassType"][2].set(types[0])

    def buttonCbk(self):
        text = [
            self.content["fileid"][2].get(),
            self.content["picid"][2].get(),
            self.content["times"][2].get(),
            self.content["entX"][2].get(),
            self.content["entY"][2].get(),
            self.content["entZ"][2].get(),
            self.classType["classType"][self.content["cboxClassType"][2].get()],
        ]
        try:
            message = f"create {float(text[0])} {float(text[1])} {float(text[2])} {float(text[3])} {float(text[4])} {float(text[5])} {float(text[6])}"
            cl.sendMessage(message)
        except ValueError:
            root.showLog("should input number")
            
    def btnSetCbk(self):
        text = [
            self.content["fileid"][2].get(),
            self.content["picid"][2].get(),
            self.content["times"][2].get(),
            self.content["entX"][2].get(),
            self.content["entY"][2].get(),
            self.content["entZ"][2].get(),
            self.classType["classType"][self.content["cboxClassType"][2].get()],
        ]
        try:
            message = f"setActor {float(text[0])} {float(text[1])} {float(text[2])} {float(text[3])} {float(text[4])} {float(text[5])} {float(text[6])}"
            cl.sendMessage(message)
        except ValueError:
            root.showLog("should input number")


# 生成默认地图
class padSpawnDefaultLand:
    def __init__(self, page_: tk.Frame) -> None:
        self.childPage = tk.Frame(page_, width=16)
        self.childPage.pack()
        self.content = {
            "title": tk.Label(self.childPage, anchor=tk.W, text="地图起点"),
            "lableX": tk.Label(self.childPage, anchor=tk.E, text="x", width=6),
            "entryX": tk.Entry(self.childPage),
            "lableY": tk.Label(self.childPage, anchor=tk.E, text="y", width=6),
            "entryY": tk.Entry(self.childPage),
            "lableZ": tk.Label(self.childPage, anchor=tk.E, text="z", width=6),
            "entryZ": tk.Entry(self.childPage),
            "lableblockSize": tk.Label(
                self.childPage, anchor=tk.E, text="块尺寸", width=6
            ),
            "entryBlkSize": tk.Entry(self.childPage),
            "lableRows": tk.Label(self.childPage, anchor=tk.E, text="行数", width=6),
            "entryrows": tk.Entry(self.childPage),
            "lableColumns": tk.Label(self.childPage, anchor=tk.E, text="列数", width=6),
            "entrycolumns": tk.Entry(self.childPage),
            "create": tk.Button(self.childPage, text="创建"),
        }
        self.contentSet = {
            "title": (1, 1, 2, tk.W, None),  # row column span var
            "lableX": (2, 1, 1, tk.E, None),
            "entryX": (2, 2, 1, tk.E, tk.StringVar()),
            "lableY": (3, 1, 1, tk.E, None),
            "entryY": (3, 2, 1, tk.E, tk.StringVar()),
            "lableZ": (4, 1, 1, tk.E, None),
            "entryZ": (4, 2, 1, tk.E, tk.StringVar()),
            "lableblockSize": (5, 1, 1, tk.E, None),
            "entryBlkSize": (5, 2, 1, tk.E, tk.StringVar()),
            "lableRows": (6, 1, 1, tk.E, None),
            "entryrows": (6, 2, 1, tk.E, tk.StringVar()),
            "lableColumns": (7, 1, 1, tk.E, None),
            "entrycolumns": (7, 2, 1, tk.E, tk.StringVar()),
            "create": (8, 2, 1, tk.W, None),
        }
        for k, v in self.content.items():
            v.grid(
                row=self.contentSet[k][0],
                column=self.contentSet[k][1],
                columnspan=self.contentSet[k][2],
                sticky=self.contentSet[k][3],
            )
            if self.contentSet[k][3] != None:
                v.config(textvariable=self.contentSet[k][4])
        self.content["create"].config(command=self.btnCbk)

    def btnCbk(self):
        try:
            data = [
                float(self.contentSet["entryX"][4].get()),
                float(self.contentSet["entryY"][4].get()),
                float(self.contentSet["entryZ"][4].get()),
                float(self.contentSet["entryBlkSize"][4].get()),
                float(self.contentSet["entryrows"][4].get()),
                float(self.contentSet["entrycolumns"][4].get()),
            ]
        except ValueError:
            root.showLog("should input number")
            return
        message = f"createDefaultLand {data[0]} {data[1]} {data[2]} {data[3]} {data[4]} {data[5]}"
        cl.sendMessage(message)


class editorPad:
    def __init__(self):
        x = 1


##################################################################################################################
##################################################################################################################
##################################################################################################################
##################################################################################################################
def parseMessage():
    cl.tryconnect(root)
    while cl.bRnuCl:
        try:
            data = cl.sockCl.recv(1024).decode("utf-8")
            root.showLog(data)
            msgs = data.split()
        except ConnectionResetError:
            root.root.quit()
            break


def program():

    padCreateObj(root.actorpage)
    padSetPos(root.actorpage)

    
    padSpawnDefaultLand(root.landpage)

    t = threading.Thread(target=parseMessage)
    t.start()
    root.mainLoop()
    cl.bRnuCl = False
    t.join()


if __name__ == "__main__":
    program()
    cl.closeSocket()
