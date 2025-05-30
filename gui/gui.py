import tkinter as tk
import socket
import threading

class STM32GUI:
    def __init__(self, master):
        self.master = master
        master.title("STM32VLDISCOVERY Simulator")
        master.geometry("500x400")
        
        # Connection frame
        frame_conn = tk.Frame(master)
        frame_conn.pack(pady=10)
        
        tk.Label(frame_conn, text="Host:").grid(row=0, column=0)
        self.host_entry = tk.Entry(frame_conn, width=15)
        self.host_entry.grid(row=0, column=1)
        self.host_entry.insert(0, "localhost")
        
        tk.Label(frame_conn, text="Port:").grid(row=0, column=2)
        self.port_entry = tk.Entry(frame_conn, width=5)
        self.port_entry.grid(row=0, column=3)
        self.port_entry.insert(0, "1234")
        
        self.connect_btn = tk.Button(
            frame_conn, 
            text="Connect", 
            command=self.toggle_connection,
            width=10
        )
        self.connect_btn.grid(row=0, column=4, padx=10)
        
        # LED control frame
        frame_leds = tk.Frame(master)
        frame_leds.pack(pady=20)
        
        self.led1_btn = tk.Button(
            frame_leds, 
            text="LED1 ON", 
            width=10,
            command=lambda: self.send_command("LED1_ON"),
            state=tk.DISABLED
        )
        self.led1_btn.grid(row=0, column=0, padx=10)
        
        tk.Button(
            frame_leds, 
            text="LED1 OFF", 
            width=10,
            command=lambda: self.send_command("LED1_OFF"),
            state=tk.DISABLED
        ).grid(row=1, column=0, padx=10, pady=5)
        
        self.led2_btn = tk.Button(
            frame_leds, 
            text="LED2 ON", 
            width=10,
            command=lambda: self.send_command("LED2_ON"),
            state=tk.DISABLED
        )
        self.led2_btn.grid(row=0, column=1, padx=10)
        
        tk.Button(
            frame_leds, 
            text="LED2 OFF", 
            width=10,
            command=lambda: self.send_command("LED2_OFF"),
            state=tk.DISABLED
        ).grid(row=1, column=1, padx=10, pady=5)
        
        self.status_btn = tk.Button(
            frame_leds, 
            text="STATUS", 
            width=10,
            command=lambda: self.send_command("STATUS"),
            state=tk.DISABLED
        )
        self.status_btn.grid(row=0, column=2, padx=10)
        
        # Status console
        self.console = tk.Text(master, height=10, width=60)
        self.console.pack(pady=10, padx=10)
        self.console.insert(tk.END, "Disconnected. Click Connect to start\n")
        self.console.config(state=tk.DISABLED)
        
        # Socket connection
        self.sock = None
        self.connected = False
        self.receive_thread = None

    def toggle_connection(self):
        if not self.connected:
            self.connect()
        else:
            self.disconnect()

    def connect(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((
                self.host_entry.get(),
                int(self.port_entry.get())
            ))
            self.connected = True
            self.connect_btn.config(text="Disconnect")
            self.enable_controls(True)
            self.log_message("Connected to QEMU")
            
            # Start receive thread
            self.receive_thread = threading.Thread(
                target=self.receive_data,
                daemon=True
            )
            self.receive_thread.start()
        except Exception as e:
            self.log_message(f"Connection failed: {str(e)}")

    def disconnect(self):
        if self.sock:
            self.sock.close()
        self.connected = False
        self.connect_btn.config(text="Connect")
        self.enable_controls(False)
        self.log_message("Disconnected")

    def enable_controls(self, enabled):
        state = tk.NORMAL if enabled else tk.DISABLED
        self.led1_btn.config(state=state)
        self.led2_btn.config(state=state)
        self.status_btn.config(state=state)
        # Enable all LED OFF buttons
        for child in self.master.winfo_children():
            if isinstance(child, tk.Frame):
                for btn in child.winfo_children():
                    if isinstance(btn, tk.Button) and "OFF" in btn.cget("text"):
                        btn.config(state=state)

    def send_command(self, cmd):
        if self.connected:
            try:
                self.sock.sendall((cmd + "\r\n").encode())
                self.log_message(f"Sent: {cmd}")
            except Exception as e:
                self.log_message(f"Send error: {str(e)}")

    def receive_data(self):
        while self.connected:
            try:
                data = self.sock.recv(1024)
                if data:
                    self.log_message(data.decode().strip())
            except:
                break

    def log_message(self, message):
        self.console.config(state=tk.NORMAL)
        self.console.insert(tk.END, message + "\n")
        self.console.see(tk.END)
        self.console.config(state=tk.DISABLED)

if __name__ == "__main__":
    root = tk.Tk()
    gui = STM32GUI(root)
    root.mainloop()