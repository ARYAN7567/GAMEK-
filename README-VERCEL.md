# GAMEK - Vercel deployment

This version uses one PHP Serverless Function (`api/index.php`) to stay within the Vercel Hobby 12-function deployment limit.

PHP application pages are stored under `app/` with `.inc` extensions and dispatched by `api/index.php`.

Database: update `app/includes/db.inc` to use a remote MySQL/MariaDB database. `localhost` only works with a local XAMPP/MySQL server.
