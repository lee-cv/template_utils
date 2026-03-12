import os

def process_quantized_images(folder_path, output_txt="image_list.txt", prefix=""):
    """
    处理量化图像数据：
    1. 按排序重命名文件夹内的图片
    2. 将重命名后的绝对路径保存到文本文件
    
    参数：
        folder_path (str): 图片文件夹路径
        output_txt (str): 保存的路径文件名
        prefix (str): 文件名前缀，可选
    """
    if not os.path.isdir(folder_path):
        print(f"❌ 错误: {folder_path} 不是有效的文件夹路径")
        return

    # 获取所有图片文件
    image_extensions = (".png", ".jpg", ".jpeg", ".bmp", ".tiff")
    images = [f for f in os.listdir(folder_path) if f.lower().endswith(image_extensions)]

    if not images:
        print("⚠️ 未找到任何图片文件")
        return

    images.sort()
    abs_paths = []

    for idx, filename in enumerate(images, start=1):
        old_path = os.path.join(folder_path, filename)
        ext = os.path.splitext(filename)[1]
        new_name = f"{prefix}{idx:04d}{ext}"
        new_path = os.path.join(folder_path, new_name)

        if old_path != new_path:
            os.rename(old_path, new_path)

        abs_paths.append(os.path.abspath(new_path))

    output_path = os.path.join(folder_path, output_txt)
    with open(output_path, "w") as f:
        f.write("\n".join(abs_paths))

    print(f"✅ 已处理 {len(images)} 张图片")
    print(f"📄 路径列表已保存到: {output_path}")


# ====== 在这里直接填写输入 ======
folder = "/home/lee/PyPro/ultralytics/semi_robot/subset"  # 替换为你的图片文件夹路径
output_txt = "image_list.txt"          # 输出文本文件名
prefix = ""                             # 可选的文件名前缀

process_quantized_images(folder, output_txt, prefix)
