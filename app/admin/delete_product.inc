<?php
require_once __DIR__ . '/../includes/db.inc';
require_once __DIR__ . '/../includes/auth.inc';
require_admin();

$id = (int)($_GET['id'] ?? 0);
if ($id > 0) {
    $stmt = $pdo->prepare('DELETE FROM products WHERE id = ?');
    $stmt->execute([$id]);
}
header('Location: index.php');
exit;
?>
