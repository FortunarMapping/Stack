// server.js
const express = require('express');
const path = require('path');
const multer = require('multer');
const conn = require('./db');

const app = express();
const PORT = 3000;

// ตั้ง static โฟลเดอร์
app.use(express.static(path.join(__dirname, 'public')));
app.use('/uploads', express.static(path.join(__dirname, 'public', 'uploads')));

// รองรับ JSON / form data
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

// ตั้ง multer สำหรับอัปโหลดรูป
const uploadDir = path.join(__dirname, 'public', 'uploads');
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, uploadDir);
  },
  filename: (req, file, cb) => {
    const unique = Date.now() + '-' + Math.round(Math.random() * 1e9);
    cb(null, unique + path.extname(file.originalname));
  }
});
const upload = multer({ storage });

// Route: รับโพสต์ใหม่
app.post('/post', upload.single('image'), (req, res) => {
  const name = req.body.acount || 'ไม่ระบุชื่อ';
  const text = req.body.text || '';
  const color = req.body.color || 'black';
  let imagePath = '';

  if (req.file) {
    imagePath = '/uploads/' + req.file.filename;
  }

  if (!text.trim()) {
    return res.status(400).json({ error: 'ข้อความต้องไม่ว่าง' });
  }

  const sql = "INSERT INTO posts (name, text, color, image, created_at) VALUES (?, ?, ?, ?, NOW())";
  conn.query(sql, [name, text, color, imagePath], (err, result) => {
    if (err) {
      console.error('DB Error:', err);
      return res.status(500).json({ error: 'Database error' });
    }
    // ถ้าต้องการ ส่งโพสต์ทั้งหมดกลับมาหลังโพสต์เสร็จ
    conn.query("SELECT * FROM posts ORDER BY created_at DESC", (err2, posts) => {
      if (err2) {
        console.error('DB Error:', err2);
        return res.status(500).json({ error: 'Database error' });
      }
      res.json(posts);
    });
  });
});

// Route: ดึงโพสต์ทั้งหมด
app.get('/posts', (req, res) => {
  conn.query("SELECT * FROM posts ORDER BY created_at DESC", (err, results) => {
    if (err) {
      console.error('DB Error:', err);
      return res.status(500).json({ error: 'Database error' });
    }
    res.json(results);
  });
});

app.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
