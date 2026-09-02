<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';

$category = $_GET['category'] ?? '';
$search = trim($_GET['search'] ?? '');
$params = [];
$sql = 'SELECT * FROM products WHERE 1=1';

if ($category !== '') {
    $sql .= ' AND category = ?';
    $params[] = $category;
}
if ($search !== '') {
    $sql .= ' AND (name LIKE ? OR description LIKE ?)';
    $params[] = "%$search%";
    $params[] = "%$search%";
}
$sql .= ' ORDER BY created_at DESC';
$stmt = $pdo->prepare($sql);
$stmt->execute($params);
$products = $stmt->fetchAll();
$categories = $pdo->query('SELECT DISTINCT category FROM products ORDER BY category')->fetchAll();

$pageTitle = 'GAMEK Products';
require __DIR__ . '/includes/header.inc';
?>
<section class="hero">
  <div>
    <h1>GAMEK</h1>
    <p>Shop gaming controllers, audio, keyboards, mice, accessories, and gear built for everyday players and competitive setups.</p>
    <div class="actions">
      <a class="btn primary" href="#products">Shop Products</a>
      <?php if (!current_user()): ?><a class="btn" href="register.php">Create Account</a><?php endif; ?>
    </div>
  </div>
  <div class="hero-media" aria-label="Gaming setup image"></div>
</section>

<section class="container" id="products">
  <form class="toolbar" method="get">
    <div>
      <h2>Gaming Products</h2>
      <p class="muted"><?= count($products) ?> product(s) available</p>
    </div>
    <div class="actions">
      <input type="search" name="search" placeholder="Search products" value="<?= htmlspecialchars($search) ?>">
      <select name="category">
        <option value="">All categories</option>
        <?php foreach ($categories as $cat): ?>
          <option value="<?= htmlspecialchars($cat['category']) ?>" <?= $category === $cat['category'] ? 'selected' : '' ?>><?= htmlspecialchars($cat['category']) ?></option>
        <?php endforeach; ?>
      </select>
      <button class="btn primary" type="submit">Filter</button>
    </div>
  </form>

  <div class="grid">
    <?php foreach ($products as $product): ?>
      <article class="card">
        <img src="<?= htmlspecialchars($product['image_url'] ?: 'https://images.unsplash.com/photo-1511512578047-dfb367046420?auto=format&fit=crop&w=900&q=80') ?>" alt="<?= htmlspecialchars($product['name']) ?>">
        <div class="card-body">
          <p class="muted"><?= htmlspecialchars($product['category']) ?></p>
          <h3><?= htmlspecialchars($product['name']) ?></h3>
          <p><?= htmlspecialchars(substr($product['description'], 0, 100)) ?>...</p>
          <div class="row">
            <span class="price"><?= money($product['price']) ?></span>
            <span class="muted">Stock: <?= (int)$product['stock'] ?></span>
          </div>
          <div class="actions">
            <a class="btn" href="product.php?id=<?= (int)$product['id'] ?>">View</a>
            <a class="btn primary" href="cart.php?action=add&id=<?= (int)$product['id'] ?>">Add to Cart</a>
          </div>
        </div>
      </article>
    <?php endforeach; ?>
  </div>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
