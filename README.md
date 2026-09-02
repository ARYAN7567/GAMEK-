# GAMEK

GAMEK is a PHP, HTML, CSS, and MySQL gaming product store.

## Features

- Product catalog with search and category filtering
- Product detail page
- User registration and login
- User profile with phone, address, and order history
- Session cart
- Checkout with stock reduction
- Admin dashboard
- Admin product add, edit, delete, and stock quantity management
- Admin order status management

## Setup

1. Install a local PHP/MySQL stack such as XAMPP, WAMP, or Laragon.
2. Copy this `GAMEZ` folder into your web server root, for example `htdocs`.
3. Import `schema.sql` into MySQL:

   ```sql
   SOURCE C:/path/to/GAMEZ/schema.sql;
   ```

4. Update `includes/db.php` if your MySQL username or password is not `root` with a blank password.
5. Open `http://localhost/GAMEZ/index.php`.

## Admin Login

- Email: `admin@gamez.test`
- Password: `admin123`

Change this password after importing the database.
