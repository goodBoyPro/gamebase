import PySimpleGUI as sg
from PIL import Image
import os


def get_folder_from_path(file_path=""):
    return os.path.dirname(file_path)


def func(fileInput=None, fileOut=None, frameSizeH=None, frameSizeW=None):
    if fileInput == "" or fileOut == "" or frameSizeH == "":
        return False

    sizeh = int(frameSizeH)
    imgIn = Image.open(fileInput)
    
    frames = imgIn.n_frames
    sizeY = sizeh
    if frameSizeW == "0":
        sizeX = int(imgIn.size[0] / imgIn.size[1] * sizeY)
    else:
        sizeX = int(frameSizeW)
    imgOut = Image.new("RGBA", (sizeX * frames, sizeY), (0, 0, 0, 0))
    for a in range(frames):
        # imgIn.seek(a)
        imgIn.seek(frames - a - 1)
        imgInTemp = imgIn.resize((sizeX, sizeY))
        imgOut.paste(imgInTemp, (sizeX * a, 0))
    # imgOut = imgOut.resize((size * frames, size))

    imgOut.save(fileOut)
    return True


def popWindow(wind=None,str=''):
    layout = [[sg.Text(str)], [sg.Button("确定")]]
    window1 = sg.Window(
        "window",
        layout,
        element_justification="center",
        modal=True,
        location=(
            wind.current_location()[0] + 150,
            wind.current_location()[1] + 200,
        ),
    )

    while True:
        event, values = window1.read()
        if event == sg.WIN_CLOSED or event == "确定":
            break
    window1.close()


def main():
    layout = [
        [
            sg.Text(
                "图片处理器", size=(15, 1), justification="center", font=("黑体", 40)
            )
        ],
        [sg.Text("把动态图处理为序列图", font=("黑体", 20))],
        [
            sg.Input(key="fileInput", size=(30, 0), justification="left",disabled=True),
            sg.FileBrowse(
                "浏览",
                size=(6, 1),
                target="fileInput",
                file_types=(("ALL Files", "*.gif webp"),),
            ),
        ],
        [
            sg.Input(key="fileOut", size=(30, 0), justification="left",disabled=True),
            sg.FileSaveAs(
                "保存为",
                key="save",
                target="fileOut",
                file_types=(("ALL Files", "*.png"),),
                size=(6, 1),
            ),
        ],
        [
            sg.Text("帧高度"),
            sg.Input(key="heightIn", size=(5, 1)),
            sg.Text("帧宽度"),
            sg.Input(key="widthIn", default_text='0',size=(5, 1)),
        ],
        [sg.Button("处理", key="run")],
    ]
    window = sg.Window(
        "program", layout, size=(400, 500), element_justification="center"
    )

    event = window.read()
    while True:
        event, values = window.read()
        if event == None:
            break
        if event == "run":

            if (not str(values["heightIn"]).isdigit()) or (
                not str(values["widthIn"]).isdigit()
            ):
                popWindow(window,'参数应为数字')
            else:
                if func(
                    values["fileInput"],
                    values["fileOut"],
                    values["heightIn"],
                    values["widthIn"],
                ):
                    sg.popup("已处理")
                    os.system(f'start {get_folder_from_path(values["fileOut"])}')
                    # .encode('gbk')
                else:
                    popWindow(window,"请检查路径")

    window.close()


if __name__ == "__main__":
    main()
