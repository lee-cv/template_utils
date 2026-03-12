import os
import cv2
import threading
import subprocess
import tkinter as tk
from tkinter import filedialog, messagebox, ttk
from datetime import datetime
from PIL import Image, ImageTk


DEFAULT_VIDEO_DIR = "/home/lee/Videos"
DEFAULT_OUTPUT_DIR = "/home/lee/Pictures"


class VideoExtractorPro:

    def __init__(self, root):

        self.root = root
        self.root.title("工业级视频抽帧工具 Pro")
        self.root.geometry("1200x800")

        # 视频变量
        self.video_path = tk.StringVar()
        self.output_dir = tk.StringVar(value=DEFAULT_OUTPUT_DIR)

        self.prefix = tk.StringVar(value="image")
        self.start_index = tk.StringVar(value="0")
        self.index_digits = tk.StringVar(value="6")

        self.time_interval = tk.StringVar(value="1")

        self.use_ffmpeg = tk.BooleanVar(value=False)
        self.create_date_folder = tk.BooleanVar(value=True)

        self.video_info = tk.StringVar(value="未加载视频")

        self.cap = None
        self.current_frame = 0
        self.total_frames = 0
        self.fps = 0

        self.start_frame = 0
        self.end_frame = 0

        # ROI
        self.roi = None
        self.roi_rect = None
        self.start_x = None
        self.start_y = None
        self.selecting = False

        # 显示比例
        self.display_w = 900
        self.display_h = 550
        self.scale_x = 1
        self.scale_y = 1

        self.build_ui()

    # ==============================
    # UI
    # ==============================

    def build_ui(self):

        left = tk.Frame(self.root)
        left.pack(side="left", fill="y", padx=10)

        right = tk.Frame(self.root)
        right.pack(side="right", fill="both", expand=True)

        # 视频
        frame = ttk.LabelFrame(left, text="视频")
        frame.pack(fill="x", pady=5)

        tk.Entry(frame, textvariable=self.video_path).pack(fill="x")

        tk.Button(frame, text="选择视频", command=self.select_video).pack()

        tk.Label(frame, textvariable=self.video_info).pack()

        # 输出
        frame = ttk.LabelFrame(left, text="输出")
        frame.pack(fill="x", pady=5)

        tk.Entry(frame, textvariable=self.output_dir).pack(fill="x")

        tk.Button(frame, text="选择目录", command=self.select_output).pack()

        tk.Checkbutton(frame, text="自动日期目录",
                       variable=self.create_date_folder).pack(anchor="w")

        # 命名
        frame = ttk.LabelFrame(left, text="命名规则")
        frame.pack(fill="x", pady=5)

        tk.Label(frame, text="前缀").grid(row=0, column=0)
        tk.Entry(frame, textvariable=self.prefix, width=10).grid(row=0, column=1)

        tk.Label(frame, text="起始编号").grid(row=1, column=0)
        tk.Entry(frame, textvariable=self.start_index, width=10).grid(row=1, column=1)

        tk.Label(frame, text="编号位数").grid(row=2, column=0)
        tk.Entry(frame, textvariable=self.index_digits, width=10).grid(row=2, column=1)

        # 抽帧参数
        frame = ttk.LabelFrame(left, text="抽帧参数")
        frame.pack(fill="x", pady=5)

        tk.Label(frame, text="每N秒一帧").pack()

        tk.Entry(frame, textvariable=self.time_interval).pack()

        tk.Checkbutton(frame,
                       text="使用FFmpeg高速抽帧",
                       variable=self.use_ffmpeg).pack(anchor="w")

        # 时间范围
        frame = ttk.LabelFrame(left, text="时间范围")
        frame.pack(fill="x", pady=5)

        tk.Button(frame, text="设置为开始帧",
                  command=self.set_start).pack(fill="x")

        tk.Button(frame, text="设置为结束帧",
                  command=self.set_end).pack(fill="x")

        # ROI
        frame = ttk.LabelFrame(left, text="ROI")
        frame.pack(fill="x", pady=5)

        tk.Label(frame, text="在右侧画框选择ROI").pack()

        tk.Button(frame, text="清除ROI",
                  command=self.clear_roi).pack(fill="x")

        # 开始按钮
        tk.Button(left,
                  text="开始抽帧",
                  font=("Arial", 14, "bold"),
                  bg="#2e8b57",
                  fg="white",
                  command=self.start_extract).pack(pady=10)

        # 右侧视频
        self.canvas = tk.Canvas(right,
                                width=self.display_w,
                                height=self.display_h,
                                bg="black")

        self.canvas.pack()

        self.canvas.bind("<ButtonPress-1>", self.on_mouse_down)
        self.canvas.bind("<B1-Motion>", self.on_mouse_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_mouse_up)

        # 信息
        self.info_label = tk.Label(right, text="")
        self.info_label.pack()

        # 进度条
        self.slider = tk.Scale(right,
                               from_=0,
                               to=100,
                               orient="horizontal",
                               command=self.on_seek)

        self.slider.pack(fill="x")

    # ==============================
    # 视频加载
    # ==============================

    def select_video(self):

        path = filedialog.askopenfilename(initialdir=DEFAULT_VIDEO_DIR)

        if not path:
            return

        self.video_path.set(path)

        self.cap = cv2.VideoCapture(path)

        self.total_frames = int(self.cap.get(cv2.CAP_PROP_FRAME_COUNT))

        self.fps = self.cap.get(cv2.CAP_PROP_FPS)

        duration = self.total_frames / self.fps

        self.video_info.set(
            f"FPS:{self.fps:.2f} Frames:{self.total_frames} Duration:{duration:.2f}s")

        self.slider.config(to=self.total_frames)

        self.show_frame()

    # ==============================
    # 显示帧
    # ==============================

    def show_frame(self):

        if not self.cap:
            return

        ret, frame = self.cap.read()

        if not ret:
            return

        self.current_frame = int(self.cap.get(cv2.CAP_PROP_POS_FRAMES))

        self.video_h, self.video_w = frame.shape[:2]

        self.scale_x = self.video_w / self.display_w
        self.scale_y = self.video_h / self.display_h

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        img = Image.fromarray(rgb)

        img = img.resize((self.display_w, self.display_h))

        self.tkimg = ImageTk.PhotoImage(img)

        self.canvas.create_image(0, 0, anchor="nw", image=self.tkimg)

        self.slider.set(self.current_frame)

        time_sec = self.current_frame / self.fps

        self.info_label.config(
            text=f"Frame {self.current_frame}/{self.total_frames}  Time {time_sec:.2f}s")

    # ==============================
    # slider
    # ==============================

    def on_seek(self, value):

        if not self.cap:
            return

        frame_id = int(value)

        self.cap.set(cv2.CAP_PROP_POS_FRAMES, frame_id)

        self.show_frame()

    # ==============================
    # ROI选择
    # ==============================

    def on_mouse_down(self, event):

        self.start_x = event.x
        self.start_y = event.y
        self.selecting = True

        if self.roi_rect:
            self.canvas.delete(self.roi_rect)

        self.roi_rect = self.canvas.create_rectangle(
            self.start_x,
            self.start_y,
            self.start_x,
            self.start_y,
            outline="red",
            width=2
        )

    def on_mouse_drag(self, event):

        if not self.selecting:
            return

        self.canvas.coords(self.roi_rect,
                           self.start_x,
                           self.start_y,
                           event.x,
                           event.y)

    def on_mouse_up(self, event):

        self.selecting = False

        x1, y1, x2, y2 = self.canvas.coords(self.roi_rect)

        self.roi = (x1, y1, x2 - x1, y2 - y1)

        print("ROI:", self.roi)

    def clear_roi(self):

        self.roi = None

        if self.roi_rect:
            self.canvas.delete(self.roi_rect)

    # ==============================
    # 设置时间范围
    # ==============================

    def set_start(self):

        self.start_frame = self.current_frame
        print("Start frame:", self.start_frame)

    def set_end(self):

        self.end_frame = self.current_frame
        print("End frame:", self.end_frame)

    # ==============================
    # 输出目录
    # ==============================

    def select_output(self):

        path = filedialog.askdirectory(initialdir=DEFAULT_OUTPUT_DIR)

        if path:
            self.output_dir.set(path)

    # ==============================
    # 开始抽帧
    # ==============================

    def start_extract(self):

        thread = threading.Thread(target=self.extract)

        thread.start()

    def extract(self):

        if self.use_ffmpeg.get():
            self.extract_ffmpeg()
        else:
            self.extract_opencv()

    # ==============================
    # OpenCV抽帧
    # ==============================

    def extract_opencv(self):

        cap = cv2.VideoCapture(self.video_path.get())

        fps = cap.get(cv2.CAP_PROP_FPS)

        interval = int(float(self.time_interval.get()) * fps)

        index = int(self.start_index.get())

        digits = int(self.index_digits.get())

        prefix = self.prefix.get()

        today = datetime.now().strftime("%Y%m%d")

        out = self.output_dir.get()

        if self.create_date_folder.get():
            out = os.path.join(out, today)

        os.makedirs(out, exist_ok=True)

        cap.set(cv2.CAP_PROP_POS_FRAMES, self.start_frame)

        frame_id = self.start_frame

        while frame_id <= self.end_frame:

            ret, frame = cap.read()

            if not ret:
                break

            if frame_id % interval == 0:

                if self.roi:

                    x, y, w, h = self.roi

                    x = int(x * self.scale_x)
                    y = int(y * self.scale_y)
                    w = int(w * self.scale_x)
                    h = int(h * self.scale_y)

                    frame = frame[y:y+h, x:x+w]

                name = f"{prefix}_{today}_{index:0{digits}d}.jpg"

                cv2.imwrite(os.path.join(out, name), frame)

                index += 1

            frame_id += 1

        cap.release()

        messagebox.showinfo("完成", "抽帧完成")

    # ==============================
    # FFmpeg抽帧
    # ==============================

    def extract_ffmpeg(self):

        interval = float(self.time_interval.get())

        fps = 1 / interval

        prefix = self.prefix.get()

        digits = int(self.index_digits.get())

        today = datetime.now().strftime("%Y%m%d")

        out = self.output_dir.get()

        if self.create_date_folder.get():
            out = os.path.join(out, today)

        os.makedirs(out, exist_ok=True)

        pattern = os.path.join(out, f"{prefix}_{today}_%0{digits}d.jpg")

        cmd = [
            "ffmpeg",
            "-i",
            self.video_path.get(),
            "-vf",
            f"fps={fps}",
            pattern
        ]

        subprocess.run(cmd)

        messagebox.showinfo("完成", "FFmpeg抽帧完成")


if __name__ == "__main__":

    root = tk.Tk()

    app = VideoExtractorPro(root)

    root.mainloop()