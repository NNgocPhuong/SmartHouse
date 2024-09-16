#include "../include/System/Buffer.h"
#include "../include/System/DateTime.h"

PCHAR int_to_buf(PCHAR p, int value, bool neg) {
    while (1) {
        *(--p) = (value % 10) | 0x30;

        if (value < 10) break;
        value /= 10;
    }

    if (neg) *(--p) = '-';
    return p;
}

void Buffer::Copy(LPCHAR s) {
    while (char c = *s++) *it++ = c; // gán c tại vị trí it rồi mới tăng
    *it = 0; // chốt null
}
LPCHAR Buffer::ToString(int value) {
    bool neg = value < 0;
    if (neg) value = -value;

    PCHAR p = PCHAR(handle + capacity);
    *(--p) = 0;

    return LPCHAR(int_to_buf(p, value, neg));
}

LPCHAR Buffer::ToString(int value, int space) { // space là định dạng số chữ số, nếu nhỏ hơn thì chèn số 0 vào trước (nên áp dụng cho số dương thôi)

    PCHAR p = PCHAR(ToString(value));
    if (value >= 0)
    {
        PCHAR end = PCHAR(handle + capacity - space - 1);
        while (p > end) *(--p) = '0';
    }
    return LPCHAR(p);
}

LPCHAR Buffer::ToString(double value)
{
    bool neg = value < 0;
    if (neg) value = -value;

    PCHAR p = PCHAR(Last());
    PCHAR e = p;

    *p = 0;

    int a = int(value);
    double b = value - a;
    while (b - int(b) > 1e-38) {
        b *= 10;
    }

    p = int_to_buf(p, int(b), false);
    *(--p) = '.';

    return LPCHAR(int_to_buf(p, a, neg));
}

/// <summary>
/// Lấy số nguyên 64 bit, 
/// IT sẽ dừng ở <b>sau ký</b> tự không phải là chữ số
/// </summary>
/// <returns></returns>
long long Buffer::GetInt64() {
    long long a = 0;
    while (true) {
        char c = *it++;
        if (c < '0' || c > '9') {
            break;
        }
        a = (a << 1) + (a << 3) + (c & 15);
    }

    return a;
}
