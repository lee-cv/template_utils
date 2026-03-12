import os
import json

# 设置你的 JSON 文件所在目录路径
json_dir = '/home/lee/PyPro/ultralytics/datasets/Semi_Robot/ShaTe_LT_2025-11-18/2'  # ← 根据你的实际路径修改

# 标签替换映射：旧标签 -> 新标签（可以添加多个映射）
label_mapping = {
    "AAA": "BBB",
    # "OldLabel1": "NewLabel1",
    # "OldLabel2": "NewLabel2"
}

for filename in os.listdir(json_dir):
    if filename.endswith('.json'):
        json_path = os.path.join(json_dir, filename)

        # 提取图像文件名（与 JSON 同名，只改后缀为 .jpg）
        image_filename = os.path.splitext(filename)[0] + '.jpg'

        # 打开并修改 JSON 文件
        with open(json_path, 'r', encoding='utf-8') as f:
            try:
                data = json.load(f)
            except json.JSONDecodeError:
                print(f'⚠️ 解析失败：{filename}')
                continue

        # 设置 imagePath 为图像文件名
        data['imagePath'] = image_filename

        # 设置 imageData 为 null
        data['imageData'] = None

        # 遍历并修改标签
        for shape in data.get('shapes', []):
            old_label = shape.get('label')
            if old_label in label_mapping:
                new_label = label_mapping[old_label]
                shape['label'] = new_label
                print(f'🔄 替换标签："{old_label}" → "{new_label}" in {filename}')

        # 保存修改后的 JSON 文件
        with open(json_path, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)

        print(f'✔ 修改 {filename}：imagePath="{image_filename}"，imageData=null')
