<?php
$assetPrefix = '../';
require_once __DIR__ . '/../includes/db.inc';
require_once __DIR__ . '/../includes/auth.inc';
require_admin();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $stmt = $pdo->prepare('UPDATE orders SET status = ? WHERE id = ?');
    $stmt->execute([$_POST['status'], (int)$_POST['order_id']]);
    header('Location: orders.php');
    exit;
}

$orders = $pdo->query('SELECT orders.*, users.name, users.email FROM orders JOIN users ON users.id = orders.user_id ORDER BY orders.created_at DESC')->fetchAll();

$pageTitle = 'Orders - GAMEK';
require __DIR__ . '/../includes/header.inc';
?>
<section class="container">
  <div class="toolbar">
    <div>
      <h1>Orders</h1>
      <p class="muted">Update customer order status.</p>
    </div>
    <a class="btn" href="index.php">Back to Admin</a>
  </div>
  <div class="table-panel">
    <table>
      <thead><tr><th>Order</th><th>Customer</th><th>Total</th><th>Payment</th><th>Status</th><th>Date</th><th>Update</th><th>Bill</th></tr></thead>
      <tbody>
      <?php foreach ($orders as $order): ?>
        <tr>
          <td>#<?= (int)$order['id'] ?></td>
          <td><?= htmlspecialchars($order['name']) ?><br><span class="muted"><?= htmlspecialchars($order['email']) ?></span></td>
          <td><?= money($order['total']) ?></td>
          <td><?= htmlspecialchars($order['payment_method'] ?? 'Cash') ?></td>
          <td><?= htmlspecialchars($order['status']) ?></td>
          <td><?= htmlspecialchars($order['created_at']) ?></td>
          <td>
            <form method="post" class="actions">
              <input type="hidden" name="order_id" value="<?= (int)$order['id'] ?>">
              <select name="status">
                <?php foreach (['Pending', 'Processing', 'Completed', 'Cancelled'] as $status): ?>
                  <option value="<?= $status ?>" <?= $order['status'] === $status ? 'selected' : '' ?>><?= $status ?></option>
                <?php endforeach; ?>
              </select>
              <button class="btn primary" type="submit">Save</button>
            </form>
          </td>
          <td><a class="btn" href="../bill.php?id=<?= (int)$order['id'] ?>">Print</a></td>
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</section>
<?php require __DIR__ . '/../includes/footer.inc'; ?>
