ทำเว็บ
เปืด xampp apache, mysql ที่ mysql กด admin
ใส่คำสั่งใน sql terminal:
1.ไปลิงค์ phpmyadmin/ พิมพ์ ที่เมนู sql
CREATE DATABASE stack;
2.จากนั้นไป phpmyadmin/db_structure.php?server=1&db=stack พิมพ์ที่เมนู sql
CREATE TABLE posts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100),
    text TEXT,
    color VARCHAR(30),
    image VARCHAR(255), -- เก็บ path ของไฟล์
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

รันได้เลย

