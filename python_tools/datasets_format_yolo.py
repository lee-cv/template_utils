import os
import json
import shutil
from pathlib import Path
from sklearn.model_selection import train_test_split
from PIL import Image
from tqdm import tqdm  # ✅ 加入 tqdm

def convert_json_to_yoloseg(json_path, image_width, image_height, class_to_id):
    with open(json_path, 'r', encoding='utf-8') as f:
        data = json.load(f)

    shapes = data.get('shapes', [])
    lines = []

    for shape in shapes:
        label = shape['label']
        points = shape['points']

        if label not in class_to_id:
            class_to_id[label] = len(class_to_id)

        class_id = class_to_id[label]

        normalized_points = []
        for x, y in points:
            normalized_x = x / image_width
            normalized_y = y / image_height
            normalized_points.extend([normalized_x, normalized_y])

        line = f"{class_id} " + " ".join(f"{x:.6f}" for x in normalized_points)
        lines.append(line)

    return lines

def prepare_coco8seg_dataset(input_folder, output_folder, val_ratio=0.2):
    input_folder = Path(input_folder)
    output_folder = Path(output_folder)
    output_image_dir = output_folder / "images"
    output_label_dir = output_folder / "labels"

    images = list(input_folder.glob("*.jpg")) + list(input_folder.glob("*.png"))
    class_to_id = {}
    dataset = []

    print("🔄 正在转换 JSON -> YOLOSeg 格式 ...")
    for image_path in tqdm(images, desc="处理图像与 JSON"):
        json_path = image_path.with_suffix('.json')
        if not json_path.exists():
            continue

        with Image.open(image_path) as img:
            width, height = img.size

        yolo_lines = convert_json_to_yoloseg(json_path, width, height, class_to_id)
        if not yolo_lines:
            continue

        dataset.append((image_path, yolo_lines))

    # 划分训练和验证集
    train_set, val_set = train_test_split(dataset, test_size=val_ratio, random_state=42)

    print("📂 正在保存数据到 COCO8-seg 结构 ...")
    for subset_name, subset_data in [('train', train_set), ('val', val_set)]:
        img_dir = output_image_dir / subset_name
        lbl_dir = output_label_dir / subset_name
        img_dir.mkdir(parents=True, exist_ok=True)
        lbl_dir.mkdir(parents=True, exist_ok=True)

        for img_path, yolo_lines in tqdm(subset_data, desc=f"保存 {subset_name}"):
            dst_img_path = img_dir / img_path.name
            shutil.copy(img_path, dst_img_path)

            label_path = lbl_dir / (img_path.stem + ".txt")
            with open(label_path, 'w') as f:
                f.write('\n'.join(yolo_lines))

    # 保存类别列表
    class_file = output_folder / "classes.txt"
    with open(class_file, 'w') as f:
        for cls_name, idx in sorted(class_to_id.items(), key=lambda x: x[1]):
            f.write(f"{cls_name}\n")

    print("\n✅ 数据集准备完成！结构如下：")

    def count_files(dir_path):
        return len(list(Path(dir_path).glob("*")))

    print(f"{output_folder}")
    print(f"├── images/         ({count_files(output_image_dir/'train') + count_files(output_image_dir/'val')})")
    print(f"│   ├── train/      ({count_files(output_image_dir/'train')})")
    print(f"│   └── val/        ({count_files(output_image_dir/'val')})")
    print(f"└── labels/         ({count_files(output_label_dir/'train') + count_files(output_label_dir/'val')})")
    print(f"    ├── train/      ({count_files(output_label_dir/'train')})")
    print(f"    └── val/        ({count_files(output_label_dir/'val')})")
    print(f"\n📄 包含类别文件: {class_file}")

# 示例调用
if __name__ == '__main__':
    input_folder = "/home/lee/PyPro/ultralytics/datasets/Semi_Robot/2025-07-15/imgLabel"
    output_folder = "/home/lee/PyPro/ultralytics/datasets/Semi_Robot/2025-07-15/datasets"
    val_ratio = 0.05  # 验证集比例

    prepare_coco8seg_dataset(input_folder, output_folder, val_ratio)
