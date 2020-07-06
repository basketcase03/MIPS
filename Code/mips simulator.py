import subprocess
import tkinter as tk

def get_mips_cache():
    subprocess.run(["Files/mips cache simulator.exe"])

def get_mips_pipeline():
    subprocess.run(["Files/mips pipeline simulator.exe"])

HEIGHT = 700
WIDTH = 1040
root=tk.Tk()
root.title("MIPS Simulator")

canvas=tk.Canvas(root,height=HEIGHT,width=WIDTH)
canvas.pack()

back_img=tk.PhotoImage(file=r'Files/Images/mips.png')

back_label=tk.Label(root,image=back_img)
back_label.place(relwidth=1,relheight=1)


#frame=tk.Frame(root, bg='#80c1ff',bd=5)
#frame.place(relx=0.3,rely=0.1,relwidth=0.65,relheight=0.1,anchor='n')

lower_frame1=tk.Frame(root,bg='black',bd=5)
lower_frame1.place(relx=0.2,rely=0.35,relwidth=0.23,relheight=0.1,anchor='n')

button=tk.Button(lower_frame1,text='MIPS Pipeline',bg='white',fg='black',width=17,font=('Courier',16),command=lambda: get_mips_pipeline())
#button.place(relx=0.3,relheight=1,relwidth=0.3)
button.grid(column=1,row=0,pady=10,padx=0)

lower_frame2=tk.Frame(root,bg='black',bd=5)
lower_frame2.place(relx=0.2,rely=0.50,relwidth=0.23,relheight=0.1,anchor='n')

button=tk.Button(lower_frame2,text='MIPS Cache',bg='white',fg='black',width=17,font=('Courier',16),command=lambda: get_mips_cache())
#button.place(relx=0.3,relheight=1,relwidth=0.3)
button.grid(column=1,row=1,pady=10)


root.mainloop()
