// db.js
const mysql = require('mysql');

const conn = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '',
  database: 'stack'
});

conn.connect(err => {
  if (err) {
    console.error('Error connecting to DB:', err);
    process.exit(1);
  }
  console.log('Connected to MySQL');
});

module.exports = conn;
