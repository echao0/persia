-- phpMyAdmin SQL Dump
-- version 4.6.6deb4
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost:3306
-- Tiempo de generación: 23-12-2018 a las 10:30:27
-- Versión del servidor: 10.1.37-MariaDB-0+deb9u1
-- Versión de PHP: 7.0.33-0+deb9u1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `persia`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `dispositivos`
--

CREATE TABLE `dispositivos` (
  `id` int(11) NOT NULL,
  `ip` text NOT NULL,
  `status` text NOT NULL,
  `Time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `move` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcado de datos para la tabla `dispositivos`
--

INSERT INTO `dispositivos` (`id`, `ip`, `status`, `Time`, `move`) VALUES
(0, '192.168.3.181', 'none', '2016-09-09 16:41:16', 0),
(1, '192.168.3.171', 'ONLINE', '2018-12-22 14:30:37', 25),
(2, '192.168.3.172', 'ONLINE', '2018-12-22 14:30:45', 25),
(3, '192.168.3.173', 'ONLINE', '2018-12-22 14:30:39', 28),
(4, '192.168.3.174', 'ONLINE', '2018-12-22 14:30:51', 0),
(5, '192.168.3.175', 'ONLINE', '2018-12-22 14:30:47', 0);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `temporiza`
--

CREATE TABLE `temporiza` (
  `id` int(2) NOT NULL,
  `disp` int(2) NOT NULL,
  `hour` varchar(5) NOT NULL,
  `type` varchar(2) NOT NULL,
  `1` int(1) NOT NULL,
  `2` int(1) NOT NULL,
  `3` int(1) NOT NULL,
  `4` int(1) NOT NULL,
  `5` int(1) NOT NULL,
  `6` int(1) NOT NULL,
  `7` int(1) NOT NULL,
  `active` varchar(2) NOT NULL,
  `short` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Volcado de datos para la tabla `temporiza`
--

INSERT INTO `temporiza` (`id`, `disp`, `hour`, `type`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `active`, `short`) VALUES
(1, 1, '20:10', 's', 1, 1, 1, 1, 1, 0, 0, '0', 0),
(2, 1, '18:30', 's', 1, 1, 1, 1, 1, 0, 0, '0', 1),
(3, 1, '19:00', 's', 1, 1, 1, 1, 1, 0, 0, '0', 0),
(4, 1, '16:00', 'b', 1, 1, 1, 1, 1, 0, 0, '0', 0),
(5, 1, '00:00', 'b', 0, 0, 0, 0, 0, 0, 0, '0', 0),
(6, 1, '00:00', 'b', 0, 0, 0, 0, 0, 0, 0, '0', 0),
(7, 2, '08:15', 's', 1, 1, 1, 1, 1, 0, 0, '0', 0),
(8, 2, '18:30', 's', 1, 1, 1, 1, 1, 1, 1, '0', 1),
(9, 2, '19:00', 's', 1, 1, 1, 1, 1, 1, 1, '0', 0),
(10, 2, '15:30', 'b', 1, 1, 1, 1, 1, 1, 1, '0', 0),
(11, 2, '00:20', 'b', 1, 1, 1, 1, 0, 0, 1, '0', 0),
(12, 2, '22:27', 'b', 0, 0, 0, 0, 0, 0, 0, '0', 0),
(13, 3, '08:00', 's', 1, 1, 1, 1, 1, 0, 0, '1', 1),
(14, 3, '18:30', 's', 1, 1, 1, 1, 1, 0, 0, '0', 1),
(15, 3, '08:05', 's', 1, 1, 1, 1, 1, 0, 0, '1', 0),
(16, 3, '15:31', 'b', 1, 1, 1, 1, 1, 1, 1, '0', 0),
(17, 3, '00:11', 'b', 1, 1, 1, 1, 0, 0, 1, '1', 0),
(18, 3, '00:12', 'b', 1, 1, 1, 1, 1, 1, 1, '1', 0);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `dispositivos`
--
ALTER TABLE `dispositivos`
  ADD PRIMARY KEY (`id`);

--
-- Indices de la tabla `temporiza`
--
ALTER TABLE `temporiza`
  ADD UNIQUE KEY `id_2` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
