import socket
import time
import sys

def send_command(command, retries=3):
    # ESP8266的IP地址（在AP模式下，ESP8266的默认IP是192.168.4.1）
    HOST = '192.168.4.1'
    PORT = 8080
    
    for attempt in range(retries):
        try:
            # 创建socket连接
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                # 设置超时时间
                s.settimeout(5.0)
                s.connect((HOST, PORT))
                print(f"[尝试 {attempt+1}/{retries}] 已连接到 {HOST}:{PORT}")
                
                # 发送命令
                s.sendall(command.encode('utf-8'))
                print(f"已发送命令: {command}")
                
                # 接收响应
                data = s.recv(1024)
                response = data.decode('utf-8')
                print(f"收到响应: {response}")
                return True, response
                
        except socket.timeout:
            print(f"[尝试 {attempt+1}/{retries}] 连接超时，请检查网络连接")
        except ConnectionRefusedError:
            print(f"[尝试 {attempt+1}/{retries}] 连接被拒绝，请检查设备是否正常运行")
        except Exception as e:
            print(f"[尝试 {attempt+1}/{retries}] 连接失败: {e}")
        
        if attempt < retries - 1:
            print("正在重试...")
            time.sleep(2)
    
    return False, "连接失败，请检查设备和网络连接"

def parse_command_input(input_str):
    """解析用户输入的命令"""
    commands = input_str.split(',')
    valid_commands = []
    
    for cmd in commands:
        if '=' in cmd:
            key, value = cmd.split('=', 1)
            key = key.strip()
            value = value.strip()
            
            # 验证命令格式
            if key in ['mode', 'temp_max', 'humi_max', 'hour', 'minute', 'sec', 'power_state']:
                # 验证数值范围
                if key in ['mode', 'power_state']:
                    if value in ['0', '1']:
                        valid_commands.append(f"{key}={value}")
                    else:
                        print(f"警告: {key} 只能是 0 或 1")
                elif key in ['temp_max', 'humi_max']:
                    try:
                        val = int(value)
                        if 0 <= val <= 100:
                            valid_commands.append(f"{key}={value}")
                        else:
                            print(f"警告: {key} 应在 0-100 之间")
                    except ValueError:
                        print(f"警告: {key} 必须是数字")
                elif key in ['hour']:
                    try:
                        val = int(value)
                        if 0 <= val <= 23:
                            valid_commands.append(f"{key}={value}")
                        else:
                            print(f"警告: {key} 应在 0-23 之间")
                    except ValueError:
                        print(f"警告: {key} 必须是数字")
                elif key in ['minute', 'sec']:
                    try:
                        val = int(value)
                        if 0 <= val <= 59:
                            valid_commands.append(f"{key}={value}")
                        else:
                            print(f"警告: {key} 应在 0-59 之间")
                    except ValueError:
                        print(f"警告: {key} 必须是数字")
            else:
                print(f"警告: 未知命令 {key}")
    
    return ','.join(valid_commands)

def main():
    print("智能电表控制工具 (优化版)")
    print("==========================")
    print("命令格式:")
    print("1. 切换模式: mode=0(自动)/1(手动)")
    print("2. 设置温度阈值: temp_max=数值(0-100)")
    print("3. 设置湿度阈值: humi_max=数值(0-100)")
    print("4. 设置定时: hour=小时(0-23),minute=分钟(0-59),sec=秒(0-59)")
    print("5. 控制电源: power_state=0(关)/1(开)")
    print("示例: mode=1,temp_max=35,humi_max=75,power_state=1")
    print("==========================")
    print("输入 'help' 查看帮助")
    print("输入 'exit' 退出")
    print("==========================")
    
    while True:
        try:
            cmd_input = input("请输入命令: ")
            
            if cmd_input.lower() == 'exit':
                print("退出程序...")
                break
            elif cmd_input.lower() == 'help':
                print("命令帮助:")
                print("- mode=0: 自动模式")
                print("- mode=1: 手动模式")
                print("- temp_max=数值: 设置温度阈值")
                print("- humi_max=数值: 设置湿度阈值")
                print("- hour=数值: 设置定时小时")
                print("- minute=数值: 设置定时分钟")
                print("- sec=数值: 设置定时秒")
                print("- power_state=0: 关闭电源")
                print("- power_state=1: 开启电源")
                continue
            
            # 解析和验证命令
            parsed_cmd = parse_command_input(cmd_input)
            if not parsed_cmd:
                print("错误: 无效的命令")
                continue
            
            # 发送命令
            success, response = send_command(parsed_cmd)
            if success:
                print("命令发送成功!")
            else:
                print(f"错误: {response}")
            
            time.sleep(1)
            
        except KeyboardInterrupt:
            print("\n用户中断，退出程序...")
            break
        except Exception as e:
            print(f"程序错误: {e}")
            time.sleep(1)

if __name__ == "__main__":
    main()
