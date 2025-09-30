import re
import numpy as np
import matplotlib.pyplot as plt

file_path = "normal_pose_result_2025-08-12-14-20.txt"

# 正则模式匹配
pattern_time = re.compile(r"---- (.*?) ----")
pattern_normal = re.compile(r"pv_normal: ([\-\d\.]+) ([\-\d\.]+) ([\-\d\.]+)")
pattern_position = re.compile(r"pv_pose_position: ([\-\d\.]+) ([\-\d\.]+) ([\-\d\.]+)")
pattern_rotation = re.compile(r"pv_pose_rotation: ([\-\d\.]+) ([\-\d\.]+) ([\-\d\.]+)")
pattern_valid = re.compile(r"valid: (\d+)")

times, normals, positions, rotations, valids = [], [], [], [], []

# 读取数据
with open(file_path, "r") as f:
    lines = f.readlines()

for line in lines:
    if match := pattern_time.search(line):
        times.append(match.group(1))
    elif match := pattern_normal.search(line):
        normals.append(list(map(float, match.groups())))
    elif match := pattern_position.search(line):
        positions.append(list(map(float, match.groups())))
    elif match := pattern_rotation.search(line):
        rotations.append(list(map(float, match.groups())))
    elif match := pattern_valid.search(line):
        valids.append(int(match.group(1)))

# 转 numpy 数组
normals = np.array(normals)
positions = np.array(positions)
rotations = np.array(rotations)
valids = np.array(valids)

# 打印统计信息
def print_stats(name, arr):
    print(f"\n=== {name} ===")
    print("Mean:", np.mean(arr, axis=0))
    print("Std Dev:", np.std(arr, axis=0))
    print("Min:", np.min(arr, axis=0))
    print("Max:", np.max(arr, axis=0))
    print("Range (Max-Min):", np.max(arr, axis=0) - np.min(arr, axis=0))

print_stats("PV Normal (原始)", normals)
print_stats("PV Position (原始)", positions)
print_stats("PV Rotation (原始)", rotations)

# 去异常值（均值 ± 3×标准差）
positions_filtered = positions.copy()
mean_pos = np.mean(positions, axis=0)
std_pos = np.std(positions, axis=0)

for i in range(3):
    mask = np.abs(positions[:, i] - mean_pos[i]) > 3 * std_pos[i]
    positions_filtered[mask, i] = np.nan

print_stats("PV Position (去异常)", positions_filtered)

print("\nValid ratio:", np.sum(valids) / len(valids) if len(valids) > 0 else 0)

# 绘图
fig, axs = plt.subplots(3, 1, figsize=(10, 8))
labels = ['X', 'Y', 'Z']

# PV Normal
for idx in range(3):
    axs[0].plot(normals[:, idx], label=f'Normal {labels[idx]}')
axs[0].set_title("PV Normal Stability")

# PV Position (去异常值版本)
for idx in range(3):
    axs[1].plot(positions_filtered[:, idx], label=f'Position {labels[idx]}')
axs[1].set_title("PV Position Stability (Filtered)")
axs[1].set_ylim(-1.5, 2.5)  # 限制显示范围

# PV Rotation
for idx in range(3):
    axs[2].plot(rotations[:, idx], label=f'Rotation {labels[idx]}')
axs[2].set_title("PV Rotation Stability")

# 统一设置
for ax in axs:
    ax.legend()
    ax.grid(True)

plt.tight_layout()
plt.show()

