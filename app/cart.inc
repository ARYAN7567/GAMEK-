<?php
require_once __DIR__ . '/includes/db.inc';
require_once __DIR__ . '/includes/auth.inc';

$_SESSION['cart'] = $_SESSION['cart'] ?? [];
$action = $_GET['action'] ?? '';
$id = (int)($_GET['id'] ?? 0);

if ($action === 'add' && $id > 0) {
    $stmt = $pdo->prepare('SELECT stock FROM products WHERE id = ?');
    $stmt->execute([$id]);
    $product = $stmt->fetch();
    if ($product && (int)$product['stock'] > ($_SESSION['cart'][$id] ?? 0)) {
        $_SESSION['cart'][$id] = ($_SESSION['cart'][$id] ?? 0) + 1;
    }
    header('Location: cart.php');
    exit;
}

if ($action === 'remove' && $id > 0) {
    unset($_SESSION['cart'][$id]);
    header('Location: cart.php');
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    foreach ($_POST['qty'] ?? [] as $productId => $qty) {
        $qty = max(0, (int)$qty);
        if ($qty === 0) {
            unset($_SESSION['cart'][$productId]);
        } else {
            $_SESSION['cart'][$productId] = $qty;
        }
    }
    header('Location: cart.php');
    exit;
}

$items = [];
$total = 0;
if ($_SESSION['cart']) {
    $ids = array_keys($_SESSION['cart']);
    $placeholders = implode(',', array_fill(0, count($ids), '?'));
    $stmt = $pdo->prepare("SELECT * FROM products WHERE id IN ($placeholders)");
    $stmt->execute($ids);
    foreach ($stmt->fetchAll() as $product) {
        $qty = min((int)$_SESSION['cart'][$product['id']], (int)$product['stock']);
        $lineTotal = $qty * (float)$product['price'];
        $total += $lineTotal;
        $items[] = ['product' => $product, 'qty' => $qty, 'line_total' => $lineTotal];
    }
}

$pageTitle = 'Cart - GAMEK';
require __DIR__ . '/includes/header.inc';
?>
<section class="container">
  <div class="toolbar">
    <div>
      <h1>Your Cart</h1>
      <p class="muted"><?= count($items) ?> item(s)</p>
    </div>
    <a class="btn" href="index.php">Continue Shopping</a>
  </div>

  <?php if (!$items): ?>
    <p class="muted">Your cart is empty.</p>
  <?php else: ?>
    <form method="post" class="table-panel">
      <table>
        <thead><tr><th>Product</th><th>Price</th><th>Qty</th><th>Total</th><th></th></tr></thead>
        <tbody>
        <?php foreach ($items as $item): $product = $item['product']; ?>
          <tr>
            <td><?= htmlspecialchars($product['name']) ?><br><span class="muted">Stock: <?= (int)$product['stock'] ?></span></td>
            <td><?= money($product['price']) ?></td>
            <td><input type="number" min="0" max="<?= (int)$product['stock'] ?>" name="qty[<?= (int)$product['id'] ?>]" value="<?= (int)$item['qty'] ?>"></td>
            <td><?= money($item['line_total']) ?></td>
            <td><a class="btn danger" href="cart.php?action=remove&id=<?= (int)$product['id'] ?>">Remove</a></td>
          </tr>
        <?php endforeach; ?>
        </tbody>
      </table>
      <div class="card-body row">
        <strong>Total: <?= money($total) ?></strong>
        <div class="actions">
          <button class="btn" type="submit">Update Cart</button>
          <a class="btn primary" href="checkout.php">Checkout</a>
        </div>
      </div>
    </form>
  <?php endif; ?>
</section>
<?php require __DIR__ . '/includes/footer.inc'; ?>
