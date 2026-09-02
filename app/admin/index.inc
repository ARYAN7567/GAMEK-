<?php
$assetPrefix = '../';
require_once __DIR__ . '/../includes/db.inc';
require_once __DIR__ . '/../includes/auth.inc';
require_admin();

$productCount = $pdo->query('SELECT COUNT(*) FROM products')->fetchColumn();
$stockCount = $pdo->query('SELECT COALESCE(SUM(stock), 0) FROM products')->fetchColumn();
$orderCount = $pdo->query('SELECT COUNT(*) FROM orders')->fetchColumn();
$products = $pdo->query('SELECT * FROM products ORDER BY created_at DESC')->fetchAll();

$pageTitle = 'Admin Panel - GAMEK';
require __DIR__ . '/../includes/header.inc';
?>
<section class="container">
  <div class="toolbar">
    <div>
      <h1>Admin Panel</h1>
      <p class="muted">Manage GAMEK products, quantities, and orders.</p>
    </div>
    <div class="actions">
      <a class="btn primary" href="product_form.php">Add Product</a>
      <a class="btn" href="orders.php">Orders</a>
      <a class="btn" href="report.php">Reports</a>
    </div>
  </div>

  <div class="grid">
    <div class="card"><div class="card-body"><p class="muted">Products</p><h2><?= (int)$productCount ?></h2></div></div>
    <div class="card"><div class="card-body"><p class="muted">Total Stock</p><h2><?= (int)$stockCount ?></h2></div></div>
    <div class="card"><div class="card-body"><p class="muted">Orders</p><h2><?= (int)$orderCount ?></h2></div></div>
  </div>

  <h2>Products</h2>
  <div class="table-panel">
    <table>
      <thead><tr><th>Name</th><th>Category</th><th>Price</th><th>No. of Product</th><th>Stock Status</th><th>Actions</th></tr></thead>
      <tbody>
      <?php foreach ($products as $product): ?>
        <tr>
          <td><?= htmlspecialchars($product['name']) ?></td>
          <td><?= htmlspecialchars($product['category']) ?></td>
          <td><?= money($product['price']) ?></td>
          <td><?= (int)$product['stock'] ?></td>
          <td><?= (int)$product['stock'] === 0 ? 'Out of Stock' : ((int)$product['stock'] <= 5 ? 'Low Stock' : 'In Stock') ?></td>
          <td class="actions">
            <a class="btn warning" href="product_form.php?id=<?= (int)$product['id'] ?>">Edit</a>
            <a class="btn danger" href="delete_product.php?id=<?= (int)$product['id'] ?>" onclick="return confirm('Delete this product?')">Delete</a>
          </td>
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</section>
<?php require __DIR__ . '/../includes/footer.inc'; ?>
