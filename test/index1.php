<?php
require("connect.php");
echo "<title>เว็บสเต็กง่ายๆ</title>";
// ดึงโพสต์ล่าสุด
$result = $conn->query("SELECT * FROM posts ORDER BY id DESC");
$posts = [];
while($row = $result->fetch_assoc()){
    $posts[] = $row;
}
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
        <form action="post.php" method="post">
            <input type=text placeholder= ผู้โพสต์ name=acount class="poster">
            <textarea name="text" placeholder="เขียนอะไรสักอย่าง..." required></textarea>
            <select name="color">
                <option value="black">ดำ</option>
                <option value="red">แดง</option>
                <option value="green">เขียว</option>
                <option value="blue">ฟ้า</option>
            </select>
            <button type="submit">โพสต์</button>
        </form>
    </section>

    <section class="feed">
        <h2>ฟีด</h2>
        <?php if(empty($posts)): ?>
            <p class="empty">ยังไม่มีโพสต์ ลองโพสต์ข้อความแรกดู</p>
        <?php else: ?>
            <?php foreach($posts as $p): ?>
                <div class="post">
                    <small>สร้างเมื่อ :<?php echo $p['created_at']; ?></small>
                    <p style="color: <?php echo htmlspecialchars($p['color']); ?>;">
                        <?php echo nl2br(htmlspecialchars($p['text'])); ?>
                        <br><small>โพสต์โดย :<?php echo $p['name']; ?></small>
                    </p>
                </div>
            <?php endforeach; ?>
        <?php endif; ?>
    </section>
</div>
</body>
</html>
