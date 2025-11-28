# 🧾 HỆ THỐNG ĐĂNG NHẬP – ĐĂNG KÝ VÀ QUẢN LÝ VÍ ĐIỂM

## 🧩 Giới thiệu dự án

Dự án xây dựng một hệ thống phần mềm bằng C++ phục vụ cho việc:
- Đăng ký và đăng nhập tài khoản người dùng với xác thực bảo mật cao (có OTP).
- Phân quyền người dùng (thông thường và quản lý).
- Quản lý thông tin cá nhân, thay đổi mật khẩu, theo dõi lịch sử hoạt động.
- Hệ thống ví điểm với khả năng chuyển điểm giữa các ví, có xác thực OTP, bảo đảm tính toàn vẹn (giao dịch atom).
- Lưu trữ thông tin vào tệp và sao lưu dữ liệu đầy đủ.

## 📌 Đặc tả chức năng

### 1. Tài khoản người dùng
#### 1.1. Đăng ký tài khoản
##### Mô tả:
Cho phép người dùng mới tạo tài khoản với tên đăng nhập duy nhất. 
##### Dữ liệu đầu vào:
- `username`: Tên đăng nhập duy nhất
- `email`: Địa chỉ email để gửi OTP
- `fullName`: Họ tên đầy đủ

##### Luồng xử lý:
1. Kiểm tra `username` đã tồn tại chưa.
2. Nếu hợp lệ, tạo mới tài khoản:
   - Băm mật khẩu bằng SHA-256
   - Tạo ví điểm rỗng kèm theo

##### Ràng buộc:
- `username` phải duy nhất

#### 1.2. Đăng nhập
##### Mô tả:
Cho phép người dùng đã có tài khoản truy cập hệ thống.

##### Dữ liệu đầu vào:
- `username`
- `password` (đã băm để so sánh)

##### Luồng xử lý:
1. Kiểm tra checklogin
2. Nếu thành công → nạp thông tin ví, vai trò, lịch sử giao dịch.

##### Ràng buộc:
- Sai mật khẩu → báo sai

#### 1.3. Cập nhật thông tin cá nhân
##### Mô tả:
Cho phép người dùng thay đổi thông tin cá nhân (email, họ tên) sau khi xác thực bằng OTP.

##### Dữ liệu đầu vào:
- Mã OTP xác nhận
- `email`, `phone` mới

##### Luồng xử lý:
1. Gửi mã OTP lên màn hình
2. Người dùng nhập OTP.
3. Nếu hợp lệ → cập nhật `email`, `phone`.

##### Ràng buộc:
- OTP chỉ được nhập 3 lần.
- Email mới phải hợp lệ

#### 1.4. Đổi mật khẩu
##### Mô tả:
Cho phép người dùng thay đổi mật khẩu vì lý do bảo mật.
##### Dữ liệu đầu vào:
- `mat khau hien tai`
- `mat khau moi`


##### Luồng xử lý:
- Cập nhật `hashedPassword` mới.


#### 1.5. Phân quyền người dùng

##### Mô tả:
Cho phép xác định và xử lý hành vi khác nhau tùy theo vai trò người dùng: `"user"` hoặc `"admin"`.

##### Vai trò:
- `"user"`:
  - Truy cập và cập nhật thông tin cá nhân
  - Xem ví điểm
  - Thực hiện giao dịch điểm
- `"admin"`:
  - Tạo tài khoản hộ người dùng
  - Xem và quản lý tất cả người dùng
  - Xem và can thiệp vào ví điểm người khác
  - Cập nhật thông tin hộ (yêu cầu OTP người dùng)

##### Ràng buộc:
- Phải đăng nhập để xác định vai trò (admin, admin123)
- Các thao tác yêu cầu quyền `admin` phải có phân quyền kiểm tra rõ ràng

### 2. Bảo mật
#### 2.1. Lưu mật khẩu an toàn

##### Mô tả:
Mật khẩu người dùng không được lưu dưới dạng thô (plain text) mà phải băm (hash) bằng thuật toán bảo mật.

##### Yêu cầu:
- Sử dụng thuật toán SHA-256 (có thể mở rộng thành SHA-512 hoặc thêm salt trong tương lai).
- Mỗi khi người dùng đăng ký hoặc đổi mật khẩu, hệ thống thực hiện.

##### Lý do bảo mật:
- Ngăn việc rò rỉ mật khẩu thực tế khi tệp dữ liệu bị truy cập trái phép.
- Hạn chế tấn công dò mật khẩu bằng từ điển (dictionary attack).

#### 2.2. Xác thực bằng OTP (One-Time Password)

##### Mô tả:
Hệ thống sinh và gửi mã OTP để xác minh các hành động quan trọng từ người dùng.

##### Quy trình:
1. Hệ thống tạo mã OTP ngẫu nhiên gồm 6 chữ số:
2. Gửi OTP đến màn hình console.
3. Khi người dùng nhập OTP:
   - So sánh với mã đã sinh.
   - Nếu khớp → cho phép thực hiện hành động.

##### Ứng dụng:
- Đăng ký tài khoản.
- Đổi mật khẩu.
- Cập nhật thông tin cá nhân.
- Giao dịch ví điểm.

##### Yêu cầu:
- Chỉ cho phép nhập lại OTP tối đa 3 lần.

#### 2.3. Bảo vệ hành động nhạy cảm

##### Danh sách hành động yêu cầu OTP:
| Hành động                         | Yêu cầu OTP | Ghi chú thêm                               |
|----------------------------------|-------------|--------------------------------------------|
| Đăng ký tài khoản                | ❌          | Không cần OTP    |
| Đổi mật khẩu                     | ❌          | Không cần OTP                  |
| Cập nhật thông tin cá nhân       | ✅          | Ngăn chỉnh sửa thông tin email/Họ tên      |
| Giao dịch chuyển điểm            | ✅          | Tránh lạm dụng và giả mạo giao dịch        |
| Tạo tài khoản hộ (admin)         | ❌          | Không cần OTP nhưng cần quyền admin        |
| Cập nhật hộ thông tin (admin)    | ✅          | Cần OTP của người dùng bị sửa              |


#### 2.4. Bảo mật lớp ứng dụng

##### Biện pháp:
- Không in trực tiếp mật khẩu vào console hoặc file log.
- Kiểm tra quyền (`role`) trước khi thực thi hành động có giới hạn.

### 3. Quản lý ví điểm
#### 3.1. Khởi tạo ví điểm

##### Mô tả:
Mỗi người dùng sẽ được tạo một ví điểm duy nhất khi đăng ký thành công.


3.1. Khởi tạo ví
####Luồng xử lý:

1.Khi tạo tài khoản mới:

- wallet trong class User luôn khởi tạo = 0.0 (đúng như constructor của bạn).

- Mỗi User chỉ có 1 ví: chính là biến wallet.

- Lịch sử giao dịch lưu bằng appendTransaction({ ... }).

####Ràng buộc:

- Một user chỉ có 1 ví → wallet là duy nhất cho mỗi User.

- Ví gắn chặt vào tài khoản → thuộc tính của class User.

####3.2. Kiểm tra số dư
Dữ liệu đầu vào:

- User A (người gửi)

- amount

####Luồng xử lý:

- Đọc giá trị:
A.getWallet()

Kiểm tra điều kiện:

if (A.getWallet() >= amount)
    // đủ tiền
else
    // không đủ tiền

####Ràng buộc:

Nếu A.wallet < amount → dừng giao dịch và báo:
"Low balance. Cannot proceed."

####3.3. Giao dịch chuyển điểm
Dữ liệu đầu vào:

- A = curr → người gửi

- B = users[receiverIndex] → người nhận

- amount

- OTP người gửi nhập

####Luồng xử lý:

1. Kiểm tra tồn tại user nhận thông qua:
2. findUserIndex(users, username)

3. Kiểm tra:

if (A.getWallet() >= amount)


4. Sinh OTP và yêu cầu người gửi nhập.

5. Nếu OTP đúng:

Trừ tiền người gửi:

A.setWallet(A.getWallet() - amount);


Cộng tiền người nhận:

B.setWallet(B.getWallet() + amount);


Ghi giao dịch vào file qua:

appendTransaction(...)


Nếu bất kỳ bước nào lỗi → rollback về giá trị cũ.

####Ràng buộc:

OTP phải trùng khớp 100%.

Không cho phép amount <= 0.

Tính toàn vẹn dữ liệu:

Trước khi chuyển luôn lưu:

oldA = A.wallet;
oldB = B.wallet;


Nếu lỗi →

A.wallet = oldA;
B.wallet = oldB;
saveAllUsers(users);


Không có trạng thái dở dang.

####3.5. Lịch sử giao dịch
Dữ liệu đầu vào:

- username

####Luồng xử lý:

1.Đọc file log giao dịch (transactions.txt).

2.Lọc các giao dịch:

From == username

hoặc To == username

3.Hiển thị theo thứ tự thời gian.

####Ràng buộc:

Người dùng chỉ xem giao dịch liên quan đến chính họ.

Admin (role == "admin") có thể xem giao dịch của bất kỳ ai.
## 📋 Bảng Cấu trúc Dữ liệu

Bảng dưới đây mô tả chi tiết các lớp dữ liệu chính được sử dụng trong hệ thống: `User`, `Wallet`, `Transaction`, `OTPManager`, và `DataManager`.  
Mỗi bảng con tương ứng với một lớp, liệt kê các thuộc tính hoặc phương thức chính, cùng với kiểu dữ liệu và mô tả rõ ràng giúp lập trình viên dễ dàng triển khai, mở rộng và bảo trì hệ thống.

---
## 📥 Cách tải và dịch chương trình

### Yêu cầu:
- Compiler hỗ trợ C++11 hoặc cao hơn (g++, clang++)
- Thư viện chuẩn C++ (không dùng thư viện ngoài)

### Tải source code:
```bash
https://github.com/lehaiduong2303-afk/DanhSachNhanVien
```

### Thao tác chính:
1. Đăng ký tài khoản
2. Đăng nhập
3. Với người dùng thường:
   - Cập nhật thông tin cá nhân (yêu cầu xác nhận OTP)
   - Thay đổi mật khẩu
   - Xem ví điểm và lịch sử giao dịch
   - Thực hiện chuyển điểm sang ví khác (OTP bắt buộc)
4. Với người quản lý:
   - Xem danh sách tài khoản
   - Tạo tài khoản hộ người dùng
   - Cập nhật thông tin hộ (yêu cầu OTP người dùng)
   - Theo dõi và quản lý ví điểm

## 🔐 Tệp tin và thư viện kèm theo

- `main.cpp` – chương trình chính
- `user.h / user.cpp` – quản lý người dùng
- `wallet.h / wallet.cpp` – quản lý ví
- `transaction.h / transaction.cpp` – xử lý giao dịch
- `utils.h / utils.cpp` – tiện ích (hash, sinh OTP,...)

## 📚 Tài liệu tham khảo

1. Tài liệu về OTP:  
   - [Wikipedia - One-time password](https://en.wikipedia.org/wiki/One-time_password)  
   - [RFC 4226 - HOTP: An HMAC-Based One-Time Password Algorithm](https://datatracker.ietf.org/doc/html/rfc4226)  
   - [RFC 6238 - TOTP: Time-Based One-Time Password Algorithm](https://datatracker.ietf.org/doc/html/rfc6238)

2. Tài liệu về bảo mật mật khẩu:
   - [OWASP Password Storage Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)
   - StackOverflow, GeeksForGeeks: các thuật toán băm như SHA-256 trong C++
