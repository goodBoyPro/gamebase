import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
import socket
import os
import time
import threading


class client:
    def __init__(self) -> None:
        self.port = 9000
        self.host = "127.0.0.1"
        self.bRnuCl = True
        # self.tryconnect()

    def tryconnect(self, root_):
        self.root = root_
        self.sockCl = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while self.bRnuCl:
            try:
                self.sockCl.connect((self.host, self.port))
                root.showLog("server connected")
                print("server connected")
                return
            except ConnectionRefusedError:
                root.showLog("retry connect")
                print("retry connect")
                time.sleep(0.5)

    def sendMessage(self, message: str):
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
        self.root = tk.Tk()
        self.root.title("editor")
        self.root.geometry("400x800+1420+100")
        self.menupage = tk.Frame(self.root, height=10,borderwidth=2, relief="groove")
        self.menupage.pack(anchor=tk.W,fill='x')
        menuBtns = {
            "save": tk.Button(self.menupage, text="保存"),
            "open": tk.Button(self.menupage, text="打开"),
        }
        btnsSet = {
            "save":(1,1),
            "open":(1,2)
        }
        for k,v in menuBtns.items():
            v.grid(row=btnsSet[k][0],column=btnsSet[k][1])

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


cl = client()
root = mainPad()


# 设置位置
class padSetPos:
    def __init__(self, page_: tk.Frame) -> None:
        self.childPage = tk.Frame(page_, borderwidth=2, relief="groove")
        self.childPage.pack()
        self.content = {
            "title": (tk.Label(self.childPage, text="位置"), (1, 2, 2), None),
            "l1": (tk.Label(self.childPage, text="x",width=4,anchor=tk.E), (2, 1, 1), None),
            "input1": (tk.Entry(self.childPage), (2, 2, 1), tk.StringVar()),
            "l2": (tk.Label(self.childPage, text="y",width=4,anchor=tk.E), (3, 1, 1), None),
            "input2": (tk.Entry(self.childPage), (3, 2, 1), tk.StringVar()),
            "l3": (tk.Label(self.childPage, text="z",width=4,anchor=tk.E), (4, 1, 1), None),
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
        self.content = {
            "title": (
                tk.Label(self.childPage, text="创建对象"),
                (1, 1, 2, tk.W),
                None,
            ),  # (row column columnspan sticky),variable
            "l1": (tk.Label(self.childPage, text="图ID"), (2, 1, 1, tk.W), None),
            "fileid": (tk.Entry(self.childPage), (2, 2, 1, tk.W), tk.StringVar()),
            "l2": (tk.Label(self.childPage, text="序号"), (3, 1, 1, tk.W), None),
            "picid": (
                tk.Entry(
                    self.childPage,
                ),
                (3, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "l3": (tk.Label(self.childPage, text="数量"), (4, 1, 1, tk.W), None),
            "times": (
                tk.Entry(
                    self.childPage,
                ),
                (4, 2, 1, tk.W),
                tk.StringVar(),
            ),
            "button": (tk.Button(self.childPage, text="创建"), (5, 2, 2, tk.W), None),
        }
        for v in self.content.values():
            v[0].grid(row=v[1][0], column=v[1][1], sticky=v[1][3], columnspan=v[1][2])
            if v[2] != None:
                v[0].config(textvariable=v[2])
        self.content["button"][0].config(command=self.buttonCbk)

    def buttonCbk(self):
        text = [
            self.content["fileid"][2].get(),
            self.content["picid"][2].get(),
            self.content["times"][2].get(),
        ]
        try:
            message = f"create {float(text[0])} {float(text[1])} {float(text[2])}"
            cl.sendMessage(message)
        except ValueError:
            root.showLog("should input number")
            return


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
    padCreateObj(root.landpage)
    padSetPos(root.actorpage)

    t = threading.Thread(target=parseMessage)
    t.start()
    root.mainLoop()
    cl.bRnuCl = False
    t.join()


if __name__ == "__main__":
    program()
    cl.closeSocket()
