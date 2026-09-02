<?php
// Single PHP entrypoint for Vercel Hobby (keeps the deployment at one Serverless Function).
$requested = $_GET['__page'] ?? null;
if ($requested === null || $requested === '') {
    $requested = parse_url($_SERVER['REQUEST_URI'] ?? '/', PHP_URL_PATH) ?: '/';
}
$requested = '/' . ltrim($requested, '/');
$requested = preg_replace('#/+#', '/', $requested);

if ($requested === '/') {
    $requested = '/index.php';
}

// Only allow known application pages; never include arbitrary filesystem paths.
$pages = [
    '/index.php' => __DIR__ . '/../app/index.inc',
    '/login.php' => __DIR__ . '/../app/login.inc',
    '/register.php' => __DIR__ . '/../app/register.inc',
    '/logout.php' => __DIR__ . '/../app/logout.inc',
    '/product.php' => __DIR__ . '/../app/product.inc',
    '/cart.php' => __DIR__ . '/../app/cart.inc',
    '/checkout.php' => __DIR__ . '/../app/checkout.inc',
    '/profile.php' => __DIR__ . '/../app/profile.inc',
    '/bill.php' => __DIR__ . '/../app/bill.inc',
    '/admin/index.php' => __DIR__ . '/../app/admin/index.inc',
    '/admin/orders.php' => __DIR__ . '/../app/admin/orders.inc',
    '/admin/product_form.php' => __DIR__ . '/../app/admin/product_form.inc',
    '/admin/delete_product.php' => __DIR__ . '/../app/admin/delete_product.inc',
    '/admin/report.php' => __DIR__ . '/../app/admin/report.inc',
];

if (!isset($pages[$requested])) {
    http_response_code(404);
    echo '404 - Page not found';
    exit;
}

require $pages[$requested];
