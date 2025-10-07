// routes.js
const express = require('express');
const router = express.Router();
const conn = require('./db');
const multer = require('multer');
const path = require('path');
const fs = require('fs');

const uploadDir = path.join(__dirname, 'public', 'uploads');
if (!fs.existsSync(uploadDir)) {
  fs.mkdirSync(uploadDir, { recursive: true });
}

const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, uploadDir);
  },
  filename: (req, file, cb) => {
    const uniqueSuffix = Date.now() + '-' + Math.round(Math.random() * 1e9);
    cb(null, uniqueSuffix + path.extname(file.originalname));
  }
});
const upload = multer({ storage });

router.post('/post', upload.single('image'), (req, res) => {
  const name = req.body.acount || 'ไม่ระบุชื่อ';
  const text = req.body.text || '';
  const color = req.body.color || 'black';
  let imagePath = '';

  if (req.file) {
    imagePath = '/uploads/' + req.file.filename;
  }

  if (!text.trim()) {
    return res.status(400).send('ข้อความไม่ควรเป็นค่าว่าง');
  }

  const sql = "INSERT INTO posts (name, text, color, image, created_at) VALUES (?, ?, ?, ?, NOW())";
  conn.query(sql, [name, text, color, imagePath], (err, result) => {
    if (err) {
      console.error('Database error:', err);
      return res.status(500).send('Database error');
    }
    res.redirect('/');
  });
});

router.get('/posts', (req, res) => {
  const sql = "SELECT * FROM posts ORDER BY created_at DESC";
  conn.query(sql, (err, results) => {
    if (err) {
      console.error('Database error:', err);
      return res.status(500).json([]);
    }
    res.json(results);
  });
});

module.exports = router;
