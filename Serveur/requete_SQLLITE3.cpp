#include <sqlite3>

CREATE TABLE IF NOT EXISTS data (
    id INTEGER PRIMARY KEY,
    timestamp DATE,
    humidity INTEGER,
    temp INTEGER,
    pression INTEGER,
    x INTEGER,
    y INTEGER,
    z INTEGER,
    mvt BOOLEAN
);

INSERT INTO data (timestamp, humidity, temp, pression, x, y, z, mvt)
VALUES (:timestamp, :humidity, :temp, :pression, :x, :y, :z, :mvt);