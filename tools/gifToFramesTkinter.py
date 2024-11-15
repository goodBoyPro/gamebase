import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox
from PIL import Image,ImageTk

imageIn=Image


photos=[]
def updatePhoto(lable:tk.Label,img=None):      
    photo=ImageTk.PhotoImage(img)
    lable.config(image=photo)
    photos.append(photo)
   
def func(fileInput=None, fileOut=None, frameSizeH=None, frameSizeW=None):
    if fileInput == "" or fileOut == "" or frameSizeH == "":
        return 
    sizeh = int(frameSizeH)
    imgIn = Image.open(fileInput)
    
    frames = imgIn.n_frames
    sizeY = sizeh
    if frameSizeW == "0":
        sizeX = int(imgIn.size[0] / imgIn.size[1] * sizeY)
    else:
        sizeX = int(frameSizeW)
    imageOut = Image.new("RGBA", (sizeX * frames, sizeY), (0, 0, 0, 0))
    for a in range(frames):
        imgIn.seek(frames - a - 1)
        imgInTemp = imgIn.resize((sizeX, sizeY))
        imageOut.paste(imgInTemp, (sizeX * a, 0))
    imageOut.save(fileOut)
    return imageOut

def openFileDialog(entry1:tk.Entry):
    filePath=filedialog.askopenfilename()
    entry1.delete(0,tk.END)
    entry1.insert(0,filePath)
   
def openFileDirDialog(entry1:tk.Entry):
    path=filedialog.asksaveasfilename(filetypes=[("png","*.png")])
    entry1.delete(0,tk.END)
    entry1.insert(0,path+".png")
    

def program():
    global imageOut
    global photo
    root=tk.Tk()
    root.title("GIF转动画序列图")
    root.geometry("500x500")
    title=tk.Label(root,text="GIF转动画序列图",font=("黑体",40),fg="#3366ff",relief='groove',borderwidth=5)
    title.pack(pady=(5,5))
    page=tk.Frame(root)
    page.pack(pady=(10,10))
    tk.Label(page,text="源文件").grid(row=1,column=1)  
    entry=tk.Entry(page,width=40)
    entry.grid(row=1,column=2)
    tk.Button(page,text="打开",width=10,command=lambda:openFileDialog(entry)).grid(row=1,column=3)
    tk.Label(page,text="存储为").grid(row=3,column=1)   
    entry2=tk.Entry(page,width=40) 
    entry2.grid(row=3,column=2)
    
    tk.Button(page,text="存储为",width=10,command=lambda:openFileDirDialog(entry2)).grid(row=3,column=3)
    tk.Label(page,text="帧宽度").grid(row=4,column=1) 
    entryW=tk.Entry(page,width=6) 
    entryW.grid(row=4,column=2) 
    tk.Label(page,text="帧高度").grid(row=5,column=1) 
    entryH=tk.Entry(page,width=6) 
    entryH.grid(row=5,column=2)   
    
    photoLable=tk.Label(root)
    photoLable.pack(pady=10)
  
    
    
    tk.Button(page,text="预览",width=10,command=lambda:updatePhoto(photoLable,func(fileInput=entry.get(),fileOut=entry2.get(),frameSizeH=int(entryH.get()),frameSizeW=int(entryW.get())))).grid(row=5,column=3)
    
    
  
    root.mainloop()
    

if __name__=="__main__":
    program()


