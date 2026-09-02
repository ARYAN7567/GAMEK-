<?php
$assetPrefix = '../';
require_once __DIR__ . '/../includes/db.inc';
require_once __DIR__ . '/../includes/auth.inc';
require_admin();

$from = $_GET['from'] ?? date('Y-m-01');
$to = $_GET['to'] ?? date('Y-m-d');
if (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $from)) {
    $from = date('Y-m-01');
}
if (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $to)) {
    $to = date('Y-m-d');
}

$summaryStmt = $pdo->prepare(
    'SELECT COUNT(*) AS orders_count, COALESCE(SUM(total), 0) AS revenue
     FROM orders
     WHERE DATE(created_at) BETWEEN ? AND ?'
);
$summaryStmt->execute([$from, $to]);
$summary = $summaryStmt->fetch();

$paymentStmt = $pdo->prepare(
    'SELECT payment_method, COUNT(*) AS orders_count, COALESCE(SUM(total), 0) AS revenue
     FROM orders
     WHERE DATE(created_at) BETWEEN ? AND ?
     GROUP BY payment_method
     ORDER BY revenue DESC'
);
$paymentStmt->execute([$from, $to]);
$paymentRows = $paymentStmt->fetchAll();

$salesStmt = $pdo->prepare(
    'SELECT products.name, products.category, COALESCE(SUM(order_items.quantity), 0) AS sold_qty,
            COALESCE(SUM(order_items.quantity * order_items.price), 0) AS sales_total
     FROM order_items
     JOIN orders ON orders.id = order_items.order_id
     JOIN products ON products.id = order_items.product_id
     WHERE DATE(orders.created_at) BETWEEN ? AND ?
     GROUP BY products.id, products.name, products.category
     ORDER BY sold_qty DESC, sales_total DESC'
);
$salesStmt->execute([$from, $to]);
$salesRows = $salesStmt->fetchAll();

$stockRows = $pdo->query(
    'SELECT name, category, price, stock,
            CASE WHEN stock = 0 THEN "Out of Stock" WHEN stock <= 5 THEN "Low Stock" ELSE "In Stock" END AS stock_status
     FROM products
     ORDER BY stock ASC, name ASC'
)->fetchAll();

$pageTitle = 'Reports - GAMEK';
require __DIR__ . '/../includes/header.inc';
?>
<section class="container print-area">
  <div class="toolbar no-print">
    <div>
      <h1>Reports</h1>
      <p class="muted">Live stock and sales report.</p>
    </div>
    <div class="actions">
      <button class="btn primary" type="button" onclick="window.print()">Print Report</button>
      <a class="btn" href="index.php">Back to Admin</a>
    </div>
  </div>

  <form class="form-panel no-print" method="get">
    <h2>Generate Report</h2>
    <label>From</label>
    <input type="date" name="from" value="<?= htmlspecialchars($from) ?>" required>
    <label>To</label>
    <input type="date" name="to" value="<?= htmlspecialchars($to) ?>" required>
    <button class="btn primary" type="submit">Generate</button>
  </form>

  <div class="grid">
    <div class="card"><div class="card-body"><p class="muted">Orders</p><h2><?= (int)$summary['orders_count'] ?></h2></div></div>
    <div class="card"><div class="card-body"><p class="muted">Revenue</p><h2><?= money($summary['revenue']) ?></h2></div></div>
    <div class="card"><div class="card-body"><p class="muted">Report Dates</p><h2><?= htmlspecialchars($from) ?> to <?= htmlspecialchars($to) ?></h2></div></div>
  </div>

  <h2>Live Stock</h2>
  <div class="table-panel">
    <table>
      <thead><tr><th>Product</th><th>Category</th><th>Price</th><th>Stock</th><th>Status</th></tr></thead>
      <tbody>
      <?php foreach ($stockRows as $row): ?>
        <tr>
          <td><?= htmlspecialchars($row['name']) ?></td>
          <td><?= htmlspecialchars($row['category']) ?></td>
          <td><?= money($row['price']) ?></td>
          <td><?= (int)$row['stock'] ?></td>
          <td><?= htmlspecialchars($row['stock_status']) ?></td>
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>

  <h2>Sales By Product</h2>
  <div class="table-panel">
    <table>
      <thead><tr><th>Product</th><th>Category</th><th>Sold Qty</th><th>Sales</th></tr></thead>
      <tbody>
      <?php foreach ($salesRows as $row): ?>
        <tr>
          <td><?= htmlspecialchars($row['name']) ?></td>
          <td><?= htmlspecialchars($row['category']) ?></td>
          <td><?= (int)$row['sold_qty'] ?></td>
          <td><?= money($row['sales_total']) ?></td>
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>

  <h2>Payment Summary</h2>
  <div class="table-panel">
    <table>
      <thead><tr><th>Payment Method</th><th>Orders</th><th>Revenue</th></tr></thead>
      <tbody>
      <?php foreach ($paymentRows as $row): ?>
        <tr>
          <td><?= htmlspecialchars($row['payment_method'] ?? 'Cash') ?></td>
          <td><?= (int)$row['orders_count'] ?></td>
          <td><?= money($row['revenue']) ?></td>
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</section>
<?php require __DIR__ . '/../includes/footer.inc'; ?>
