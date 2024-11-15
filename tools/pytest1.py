import tkinter as tk
from tkinter import filedialog


class actorLableButtonEntry:
    def __init__(self, page_, row_, column_) -> None:
        self.lableName = []
        self.buttonName = []
        # self.entryName=['entry1','entry2','entry3']
        self.pagename = "pagename"
        self.page1 = tk.Frame(page_, width=300,height=400)
        self.page1.grid(row=row_, column=column_)
        tk.Label(self.page1, text=self.pagename).grid(row=1, column=1)

    def createPage(self):
        labels = [
            tk.Label(
                self.page1, width=10, text=self.lableName[i], justify="right"
            ).grid(row=i + 2, column=1)
            for i in range(len(self.lableName))
        ]
        self.entrys = {
            self.lableName[i]: tk.Entry(self.page1, width=20)
            for i in range(len(self.lableName))
        }
        for i in range(len(self.lableName)):
            self.entrys[self.lableName[i]].grid(row=i + 2, column=2, padx=5)
        self.buttons = {
            self.buttonName[i]: tk.Button(self.page1, width=8, text=self.buttonName[i])
            for i in range(len(self.buttonName))
        }
        for i in range(len(self.buttonName)):
            self.buttons[self.buttonName[i]].grid(row=i + 2, column=3)


def program():
    root = tk.Tk()
    root.title("program")
    root.grid(10, 10, 800, 800)
    titleLable = tk.Label(
        root,
        text="PROGRAM",
        width=10,
        font=("黑体", 40),
        fg="green",
        relief="groove",
        borderwidth=6,
    )
    titleLable.pack(pady=10)
    page = tk.Frame(root)
    page.pack(pady=10)

    ins = actorLableButtonEntry(page, 1, 1)
    ins.buttonName = ["a", "b", "c", "d", "e", "f"]
    ins.lableName = ["a", "b", "c", "d", "e", "f"]
    ins.createPage()
    for i in ins.buttonName:
        ins.buttons[i].config(
            command=lambda x=i: (
                ins.entrys[x].delete(0, tk.END),
                ins.entrys[x].insert(0, filedialog.askopenfilename())
        )
    )

    root.mainloop()


if __name__ == "__main__":
    program()
