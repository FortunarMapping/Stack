ใส่คำสั่งใน sql terminal
1.CREATE DATABASE stack;
2.CREATE TABLE posts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255),
    text TEXT,
    color VARCHAR(30),
    image VARCHAR(255), -- เก็บ path ของไฟล์
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
ใน vs code terminal 
1.npm init -y
2.npm install express MySQL
3.npm install express mysql multer
4.node server.js (คำสั่งรันโค้ด)