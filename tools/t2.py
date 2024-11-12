import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox
def openFileDialog(entry1:tk.Entry):
    filePath=filedialog.askopenfilename()
    if filePath:
        entry1.delete(0,tk.END)
        entry1.insert(0,filePath)
    else:
        messagebox.showinfo("info:","nofile")
def openFileDirDialog(entry1:tk.Entry):
    path=filedialog.asksaveasfilename(filetypes=[("png","*.png")])
    if path:
        entry1.delete(0,tk.END)
        entry1.insert(0,path+".png")
    else:
        messagebox.showinfo("info","nofile")

def program():
    root=tk.Tk()
    root.title("图片处理器")
    root.geometry("500x500")
    title=tk.Label(root,text="图片处理器",font=("黑体",40),fg="#3366ff")
    title.pack(pady=(5,5))
    page=tk.Frame(root)
    page.pack(pady=(10,10))
    tk.Label(page,text="源文件").grid(row=1,column=1)  
    entry=tk.Entry(page,width=40)
    entry.grid(row=1,column=2)
    tk.Button(page,text="打开",command=lambda:openFileDialog(entry)).grid(row=2,column=2)
    tk.Label(page,text="存储为").grid(row=3,column=1)   
    entry2=tk.Entry(page,width=40) 
    entry2.grid(row=3,column=2)
    tk.Button(page,text="存储为",command=lambda:openFileDirDialog(entry2)).grid(row=4,column=2)
   
    
    
    

    root.mainloop()
    

if __name__=="__main__":
    program()