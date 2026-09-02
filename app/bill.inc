<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';
require_login();

$orderId = (int)($_GET['id'] ?? 0);
$stmt = $pdo->prepare(
    'SELECT orders.*, users.name, users.email, users.phone, users.address
     FROM orders
     JOIN users ON users.id = orders.user_id
     WHERE orders.id = ?'
);
$stmt->execute([$orderId]);
$order = $stmt->fetch();

if (!$order || ((int)$order['user_id'] !== (int)current_user()['id'] && current_user()['role'] !== 'admin')) {
    exit('Bill not found.');
}

$itemStmt = $pdo->prepare(
    'SELECT order_items.*, products.name
     FROM order_items
     JOIN products ON products.id = order_items.product_id
     WHERE order_items.order_id = ?
     ORDER BY order_items.id'
);
$itemStmt->execute([$orderId]);
$items = $itemStmt->fetchAll();

$pageTitle = 'Bill #' . $orderId . ' - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="container print-area">
  <div class="toolbar no-print">
    <div>
      <h1>Bill #<?= (int)$order['id'] ?></h1>
      <p class="muted">Generated on <?= htmlspecialchars($order['created_at']) ?></p>
    </div>
    <div class="actions">
      <button class="btn primary" type="button" onclick="window.print()">Print Bill</button>
      <a class="btn" href="profile.php">Back to Profile</a>
    </div>
  </div>

  <div class="bill">
    <div class="row">
      <div>
        <h1>GAMEK</h1>
        <p class="muted">Gaming products invoice</p>
      </div>
      <div>
        <strong>Bill #<?= (int)$order['id'] ?></strong><br>
        <span><?= htmlspecialchars($order['created_at']) ?></span>
      </div>
    </div>

    <hr>
    <div class="grid two-col">
      <div>
        <h3>Customer</h3>
        <p><?= htmlspecialchars($order['name']) ?></p>
        <p><?= htmlspecialchars($order['email']) ?></p>
        <?php if (!empty($order['phone'])): ?><p><?= htmlspecialchars($order['phone']) ?></p><?php endif; ?>
        <?php if (!empty($order['address'])): ?><p><?= nl2br(htmlspecialchars($order['address'])) ?></p><?php endif; ?>
      </div>
      <div>
        <h3>Payment</h3>
        <p><?= htmlspecialchars($order['payment_method'] ?? 'Cash') ?></p>
        <?php if (!empty($order['payment_reference'])): ?><p><?= htmlspecialchars($order['payment_reference']) ?></p><?php endif; ?>
        <p>Status: <?= htmlspecialchars($order['status']) ?></p>
      </div>
    </div>

    <div class="table-panel">
      <table>
        <thead><tr><th>Product</th><th>Qty</th><th>Price</th><th>Total</th></tr></thead>
        <tbody>
        <?php foreach ($items as $item): ?>
          <tr>
            <td><?= htmlspecialchars($item['name']) ?></td>
            <td><?= (int)$item['quantity'] ?></td>
            <td><?= money($item['price']) ?></td>
            <td><?= money($item['quantity'] * $item['price']) ?></td>
          </tr>
        <?php endforeach; ?>
        </tbody>
      </table>
    </div>

    <div class="row bill-total">
      <strong>Total Amount</strong>
      <strong><?= money($order['total']) ?></strong>
    </div>
  </div>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
