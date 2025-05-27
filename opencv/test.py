import cv2
import numpy as np
import serial
import struct

cap = cv2.VideoCapture(1)
if not cap.isOpened():
        print("无法打开摄像头！")
ser = serial.Serial('COM5', 115200)


def send_data_packet(cx, cy, found):
    """发送数据帧到串口"""
    # 坐标限幅（确保在0-255范围内）
    cx = max(0, min(255, cx))
    cy = max(0, min(255, cy))

    # 数据打包为二进制（7字节）
    packet = struct.pack(
        'BBBBBBB',  # 格式：7个无符号字节
        0x2C,       # 帧头1
        0x12,       # 帧头2
        cx,         # X坐标
        cy,         # Y坐标
        1 if found else 0,  # Finded_flag
        0x00,       # Ch（预留）
        0x5B        # 帧尾
    )
    ser.write(packet)

def find_white_ball(image_path):
    img = image_path
    if img is None:
        return []

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 245, 255, cv2.THRESH_BINARY)

    kernel = np.ones((5, 5), np.uint8)
    binary = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)
    binary = cv2.morphologyEx(binary, cv2.MORPH_CLOSE, kernel)

    cv2.imshow('Camera', binary)

    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    centers = []

    for contour in contours:
        if 100 > cv2.contourArea(contour) > 10:
            continue
        M = cv2.moments(contour)
        if M["m00"]:
            # centers.append((int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"])))
            if M["m00"] > 0:
                raw_cx = int(M["m10"] / M["m00"])
                raw_cy = int(M["m01"] / M["m00"])
                # 按比例缩放至0-255
                cx = int(raw_cx * 255 / frame.shape[1])
                cy = int(raw_cy * 255 / frame.shape[0])
                return cx, cy, True
            return 0, 0, False
            # # 计算坐标范围
            # if centers:
            #     x_coords = [c[0] for c in centers]
            #     y_coords = [c[1] for c in centers]
            #
            #     min_x, max_x = min(x_coords), max(x_coords)
            #     min_y, max_y = min(y_coords), max(y_coords)
            #
            #     print(f"X范围: {min_x} - {max_x}, Y范围: {min_y} - {max_y}")

    return centers

while cap.isOpened():
    _, frame = cap.read()

    # centers = find_white_ball(frame)

    # 检测小球
    cx, cy, found = find_white_ball(frame)
    # 发送数据帧
    send_data_packet(cx, cy, found)

    # 显示结果（调试用）
    cv2.putText(frame, f"X: {cx}, Y: {cy}", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
    cv2.imshow('Camera', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


    # fixed = f"{centers[0]}, {centers[1]}\n"
    # ser.write(fixed.encode())

    # import cv2
    #
    # # 打开摄像头（0通常是默认摄像头）
    # cap = cv2.VideoCapture(0)
    #
    # while True:
    #     # 读取一帧
    #     ret, frame = cap.read()
    #
    #     # 如果读取失败则退出
    #     if not ret:
    #         print("无法获取画面")
    #         break
    #
    #     # 显示画面（窗口标题为"Camera"）
    #     cv2.imshow('Camera', frame)
    #
    #     # 按'q'键退出
    #     if cv2.waitKey(1) & 0xFF == ord('q'):
    #         break
    #
    # 释放资源
cap.release()
cv2.destroyAllWindows()