import tkinter as tk
from tkinter import filedialog, ttk
import csv

def load_csv():
    path = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
    if not path:
        return

    for row in tree.get_children():
        tree.delete(row)

    with open(path, newline="") as f:
        reader = csv.reader(f)
        header = next(reader)
        tree["columns"] = header
        tree.heading("#0", text="")
        for col in header:
            tree.heading(col, text=col)
            tree.column(col, width=80, anchor="center")

        for row in reader:
            tree.insert("", "end", text="", values=row)

root = tk.Tk()
root.title("Artemis Harness Tester Matrix Viewer")

frame = ttk.Frame(root, padding=10)
frame.pack(fill="both", expand=True)

btn = ttk.Button(frame, text="Load Matrix CSV", command=load_csv)
btn.pack(pady=5)

tree = ttk.Treeview(frame, show="headings")
tree.pack(fill="both", expand=True)

root.mainloop()
