<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';

$stmt = $pdo->prepare('SELECT * FROM products WHERE id = ?');
$stmt->execute([$_GET['id'] ?? 0]);
$product = $stmt->fetch();
if (!$product) {
    http_response_code(404);
    exit('Product not found.');
}

$pageTitle = $product['name'] . ' - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="container">
  <div class="grid">
    <img class="card" src="<?= htmlspecialchars($product['image_url'] ?: 'https://images.unsplash.com/photo-1511512578047-dfb367046420?auto=format&fit=crop&w=900&q=80') ?>" alt="<?= htmlspecialchars($product['name']) ?>">
    <div>
      <p class="muted"><?= htmlspecialchars($product['category']) ?></p>
      <h1><?= htmlspecialchars($product['name']) ?></h1>
      <p><?= nl2br(htmlspecialchars($product['description'])) ?></p>
      <p class="price"><?= money($product['price']) ?></p>
      <p class="muted">Available quantity: <?= (int)$product['stock'] ?></p>
      <div class="actions">
        <a class="btn primary" href="cart.php?action=add&id=<?= (int)$product['id'] ?>">Add to Cart</a>
        <a class="btn" href="index.php">Back</a>
      </div>
    </div>
  </div>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
