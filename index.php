<?php
require("connect.php");
echo "<title>เว็บสเต็กง่ายๆ</title>";

// ดึงโพสต์ล่าสุด
$result = $conn->query("SELECT * FROM posts ORDER BY id DESC");
?>
<!DOCTYPE html>
<html lang="th">
<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="style.css">
</head>
<body>
<div class="container">
    <header>
        <h1>เว็บสเต็ก ง่าย ๆ</h1>
        <p>โพสต์ข้อความของคุณแล้วจะเห็นทันทีในฟีด</p>
    </header>

    <section class="post-form">
        <form action="post.php" method="post" enctype="multipart/form-data">
            <input type="text" name="acount" placeholder="ผู้โพสต์" class="poster"><br>
            <textarea name="text" placeholder="เขียนอะไรสักอย่าง..." required></textarea><br>
            สีตัวอักษร:
            <select name="color">
                <option value="black">ดำ</option>
                <option value="red">แดง</option>
                <option value="green">เขียว</option>
                <option value="blue">ฟ้า</option>
            </select><br>
            แนบรูปภาพ: <input type="file" name="image"><br>
            <button type="submit">โพสต์</button>
        </form>
    </section>

    <section class="feed">
        <h2>ฟีด</h2>
        <?php if ($result->num_rows === 0): ?>
            <p>ยังไม่มีโพสต์</p>
        <?php else: ?>
            <?php while ($row = $result->fetch_assoc()): ?>
                <div style="border:1px solid #ddd; padding:10px; margin:10px 0;">
                    <strong style="color:<?= htmlspecialchars($row['color']) ?>">
                        <?= htmlspecialchars($row['name']) ?>
                    </strong>
                    <p><?= nl2br(htmlspecialchars($row['text'])) ?></p>
                    <?php if (!empty($row['image'])): ?>
                        <img src="<?= htmlspecialchars($row['image']) ?>" alt="post image" style="max-width:300px;">
                    <?php endif; ?>
                    <br><small>โพสต์เมื่อ :<?= $row['created_at'] ?></small>
                </div>
            <?php endwhile; ?>
        <?php endif; ?>
    </section>
</div>
</body>
</html>
