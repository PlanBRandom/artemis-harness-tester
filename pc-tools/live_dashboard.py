#!/usr/bin/env python3
"""
Artemis Harness Tester - Live Dashboard
Real-time serial monitoring with matrix visualization and test controls
"""

import tkinter as tk
from tkinter import ttk, scrolledtext
import serial
import serial.tools.list_ports
import threading
import queue
import json
import re
from datetime import datetime

class ArtemisLiveDashboard:
    def __init__(self, root):
        self.root = root
        self.root.title("Artemis Harness Tester - Live Dashboard")
        self.root.geometry("1200x800")
        
        self.serial_port = None
        self.serial_thread = None
        self.running = False
        self.data_queue = queue.Queue()
        
        # Test data
        self.matrix_data = [[0 for _ in range(21)] for _ in range(21)]
        self.channel_status = {}
        
        self.setup_ui()
        self.update_ports()
        
    def setup_ui(self):
        # Top toolbar
        toolbar = ttk.Frame(self.root)
        toolbar.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)
        
        ttk.Label(toolbar, text="Port:").pack(side=tk.LEFT, padx=5)
        self.port_combo = ttk.Combobox(toolbar, width=15, state='readonly')
        self.port_combo.pack(side=tk.LEFT, padx=5)
        
        ttk.Button(toolbar, text="Refresh", command=self.update_ports).pack(side=tk.LEFT, padx=2)
        
        self.connect_btn = ttk.Button(toolbar, text="Connect", command=self.toggle_connection)
        self.connect_btn.pack(side=tk.LEFT, padx=5)
        
        ttk.Separator(toolbar, orient=tk.VERTICAL).pack(side=tk.LEFT, fill=tk.Y, padx=10)
        
        # Test control buttons
        ttk.Button(toolbar, text="MEASURE", command=lambda: self.send_command("MEASURE")).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="MATRIX", command=lambda: self.send_command("MATRIX")).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="SELFTEST", command=lambda: self.send_command("SELFTEST")).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="PROFILE", command=lambda: self.send_command("PROFILE")).pack(side=tk.LEFT, padx=2)
        
        self.status_label = ttk.Label(toolbar, text="Disconnected", foreground="red")
        self.status_label.pack(side=tk.RIGHT, padx=10)
        
        # Main content area
        content = ttk.PanedWindow(self.root, orient=tk.HORIZONTAL)
        content.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Left panel - Matrix visualization
        left_panel = ttk.Frame(content)
        content.add(left_panel, weight=2)
        
        ttk.Label(left_panel, text="Connectivity Matrix (21×21)", font=('Arial', 12, 'bold')).pack(pady=5)
        
        matrix_frame = ttk.Frame(left_panel)
        matrix_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        self.matrix_canvas = tk.Canvas(matrix_frame, bg='white')
        self.matrix_canvas.pack(fill=tk.BOTH, expand=True)
        self.matrix_canvas.bind('<Configure>', self.draw_matrix)
        
        # Right panel - split into channels and console
        right_panel = ttk.PanedWindow(content, orient=tk.VERTICAL)
        content.add(right_panel, weight=1)
        
        # Channel status
        status_frame = ttk.LabelFrame(right_panel, text="Channel Status")
        right_panel.add(status_frame, weight=1)
        
        self.status_tree = ttk.Treeview(status_frame, columns=('Status', 'Resistance'), show='tree headings', height=10)
        self.status_tree.heading('#0', text='Channel')
        self.status_tree.heading('Status', text='Status')
        self.status_tree.heading('Resistance', text='Resistance (Ω)')
        self.status_tree.column('#0', width=80)
        self.status_tree.column('Status', width=80)
        self.status_tree.column('Resistance', width=100)
        
        status_scroll = ttk.Scrollbar(status_frame, orient=tk.VERTICAL, command=self.status_tree.yview)
        self.status_tree.configure(yscrollcommand=status_scroll.set)
        
        self.status_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        status_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Console output
        console_frame = ttk.LabelFrame(right_panel, text="Serial Console")
        right_panel.add(console_frame, weight=1)
        
        self.console = scrolledtext.ScrolledText(console_frame, wrap=tk.WORD, height=15, bg='black', fg='#00ff00', font=('Courier', 9))
        self.console.pack(fill=tk.BOTH, expand=True, padx=2, pady=2)
        
        # Start UI update loop
        self.root.after(100, self.process_queue)
        
    def update_ports(self):
        ports = [p.device for p in serial.tools.list_ports.comports()]
        self.port_combo['values'] = ports
        if ports:
            self.port_combo.current(0)
            
    def toggle_connection(self):
        if self.running:
            self.disconnect()
        else:
            self.connect()
            
    def connect(self):
        port = self.port_combo.get()
        if not port:
            self.log_console("ERROR: No port selected")
            return
            
        try:
            self.serial_port = serial.Serial(port, 115200, timeout=0.1)
            self.running = True
            self.serial_thread = threading.Thread(target=self.read_serial, daemon=True)
            self.serial_thread.start()
            
            self.connect_btn.config(text="Disconnect")
            self.status_label.config(text="Connected", foreground="green")
            self.log_console(f"Connected to {port} @ 115200 baud")
            
        except Exception as e:
            self.log_console(f"ERROR: {e}")
            
    def disconnect(self):
        self.running = False
        if self.serial_thread:
            self.serial_thread.join(timeout=1)
        if self.serial_port:
            self.serial_port.close()
            
        self.connect_btn.config(text="Connect")
        self.status_label.config(text="Disconnected", foreground="red")
        self.log_console("Disconnected")
        
    def send_command(self, cmd):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.write(f"{cmd}\n".encode())
            self.log_console(f"> {cmd}")
        else:
            self.log_console("ERROR: Not connected")
            
    def read_serial(self):
        buffer = ""
        while self.running:
            try:
                if self.serial_port.in_waiting:
                    chunk = self.serial_port.read(self.serial_port.in_waiting).decode('utf-8', errors='ignore')
                    buffer += chunk
                    
                    # Process complete lines
                    while '\n' in buffer:
                        line, buffer = buffer.split('\n', 1)
                        self.data_queue.put(('line', line.strip()))
                        
            except Exception as e:
                self.data_queue.put(('error', str(e)))
                break
                
    def process_queue(self):
        try:
            while True:
                msg_type, data = self.data_queue.get_nowait()
                
                if msg_type == 'line':
                    self.process_line(data)
                elif msg_type == 'error':
                    self.log_console(f"ERROR: {data}")
                    
        except queue.Empty:
            pass
            
        self.root.after(100, self.process_queue)
        
    def process_line(self, line):
        self.log_console(line)
        
        # Parse JSON responses
        if line.startswith('{') and line.endswith('}'):
            try:
                data = json.loads(line)
                
                # Matrix data
                if 'matrix' in data:
                    matrix = data['matrix']
                    if len(matrix) == 21 and all(len(row) == 21 for row in matrix):
                        self.matrix_data = matrix
                        self.draw_matrix()
                        
                # Channel measurements
                else:
                    for ch, info in data.items():
                        if ch.startswith('CH'):
                            self.update_channel_status(ch, info)
                            
            except json.JSONDecodeError:
                pass
                
    def update_channel_status(self, ch, info):
        status = info.get('status', 'UNKNOWN')
        r = info.get('r_ohms')
        r_str = f"{r:.2f}" if r is not None else "OPEN"
        
        # Update tree
        existing = self.status_tree.get_children()
        found = False
        for item in existing:
            if self.status_tree.item(item)['text'] == ch:
                self.status_tree.item(item, values=(status, r_str))
                found = True
                break
                
        if not found:
            tag = 'ok' if status == 'OK' else 'fail'
            self.status_tree.insert('', 'end', text=ch, values=(status, r_str), tags=(tag,))
            
        self.status_tree.tag_configure('ok', foreground='green')
        self.status_tree.tag_configure('fail', foreground='red')
        
    def draw_matrix(self, event=None):
        canvas = self.matrix_canvas
        canvas.delete('all')
        
        width = canvas.winfo_width()
        height = canvas.winfo_height()
        
        if width < 50 or height < 50:
            return
            
        cell_size = min((width - 40) // 21, (height - 40) // 21)
        x_offset = (width - cell_size * 21) // 2
        y_offset = (height - cell_size * 21) // 2
        
        for i in range(21):
            for j in range(21):
                x = x_offset + j * cell_size
                y = y_offset + i * cell_size
                
                value = self.matrix_data[i][j]
                
                if i == j:
                    color = '#cccccc'  # Diagonal (self)
                elif value == 1:
                    color = '#00ff00'  # Connected
                elif value == 0:
                    color = '#ff0000'  # Not connected
                else:
                    color = 'white'    # Unknown
                    
                canvas.create_rectangle(x, y, x + cell_size, y + cell_size, 
                                       fill=color, outline='black')
                                       
        # Labels
        for i in range(21):
            x = x_offset + i * cell_size + cell_size // 2
            canvas.create_text(x, y_offset - 10, text=str(i), font=('Arial', 8))
            
            y = y_offset + i * cell_size + cell_size // 2
            canvas.create_text(x_offset - 10, y, text=str(i), font=('Arial', 8))
            
    def log_console(self, text):
        timestamp = datetime.now().strftime("%H:%M:%S")
        self.console.insert(tk.END, f"[{timestamp}] {text}\n")
        self.console.see(tk.END)
        
    def on_closing(self):
        self.disconnect()
        self.root.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = ArtemisLiveDashboard(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()
