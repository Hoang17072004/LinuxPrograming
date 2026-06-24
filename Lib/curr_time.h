#ifndef CURR_TIME_H
/* Trả về chuỗi biểu diễn thời gian hiện tại.
   Nếu format = NULL, dùng định dạng mặc định "%Y-%m-%d %H:%M:%S".
   Nếu buf != NULL, kết quả sẽ được ghi vào buf (phải đủ lớn).
   Nếu buf = NULL, hàm sẽ cấp phát động và trả về con trỏ (cần free sau khi dùng). */
char *currTime(const char *format);
#endif
