<?php
$assetPrefix = '../';
require_once __DIR__ . '/../includes/db.inc';
require_once __DIR__ . '/../includes/auth.inc';
require_admin();

$id = (int)($_GET['id'] ?? 0);
$product = ['name' => '', 'category' => '', 'description' => '', 'price' => '', 'stock' => '', 'image_url' => ''];

if ($id) {
    $stmt = $pdo->prepare('SELECT * FROM products WHERE id = ?');
    $stmt->execute([$id]);
    $product = $stmt->fetch();
    if (!$product) {
        exit('Product not found.');
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $data = [
        trim($_POST['name'] ?? ''),
        trim($_POST['category'] ?? ''),
        trim($_POST['description'] ?? ''),
        (float)($_POST['price'] ?? 0),
        max(0, (int)($_POST['stock'] ?? 0)),
        trim($_POST['image_url'] ?? ''),
    ];

    if ($id) {
        $stmt = $pdo->prepare('UPDATE products SET name = ?, category = ?, description = ?, price = ?, stock = ?, image_url = ? WHERE id = ?');
        $stmt->execute(array_merge($data, [$id]));
    } else {
        $stmt = $pdo->prepare('INSERT INTO products (name, category, description, price, stock, image_url) VALUES (?, ?, ?, ?, ?, ?)');
        $stmt->execute($data);
    }
    header('Location: index.php');
    exit;
}

$pageTitle = ($id ? 'Edit' : 'Add') . ' Product - GAMEK';
require __DIR__ . '/../includes/header.inc';
?>
<section class="form-panel">
  <h1><?= $id ? 'Edit' : 'Add' ?> Product</h1>
  <form method="post">
    <label>Name</label>
    <input name="name" value="<?= htmlspecialchars($product['name']) ?>" required>
    <label>Category</label>
    <input name="category" value="<?= htmlspecialchars($product['category']) ?>" required>
    <label>Description</label>
    <textarea name="description" required><?= htmlspecialchars($product['description']) ?></textarea>
    <label>Price</label>
    <input type="number" step="0.01" min="0" name="price" value="<?= htmlspecialchars((string)$product['price']) ?>" required>
    <label>No. of Product / Stock</label>
    <input type="number" min="0" name="stock" value="<?= htmlspecialchars((string)$product['stock']) ?>" required>
    <label>Image URL</label>
    <input type="url" name="image_url" value="<?= htmlspecialchars($product['image_url'] ?? '') ?>">
    <div class="actions">
      <button class="btn primary" type="submit">Save Product</button>
      <a class="btn" href="index.php">Cancel</a>
    </div>
  </form>
</section>
<?php require __DIR__ . '/../includes/footer.inc'; ?>
