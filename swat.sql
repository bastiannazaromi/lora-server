-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Oct 18, 2024 at 03:04 AM
-- Server version: 10.4.27-MariaDB
-- PHP Version: 7.4.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `swat`
--

-- --------------------------------------------------------

--
-- Table structure for table `lokasi`
--

CREATE TABLE `lokasi` (
  `id` int(11) NOT NULL,
  `serialNumber` varchar(25) NOT NULL,
  `latitude` varchar(30) NOT NULL,
  `longitude` varchar(30) NOT NULL,
  `createdAt` timestamp NOT NULL DEFAULT current_timestamp(),
  `updatedAt` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `lokasi`
--

INSERT INTO `lokasi` (`id`, `serialNumber`, `latitude`, `longitude`, `createdAt`, `updatedAt`) VALUES
(1, 'node-01', '-6.868421511887271', '109.10775959584207', '2024-09-09 09:44:09', '2024-10-17 08:44:57'),
(2, 'node-03', '-6.868894155305693', '109.119991156406', '2024-09-09 09:44:09', '2024-10-17 08:49:12');

-- --------------------------------------------------------

--
-- Table structure for table `pelanggan`
--

CREATE TABLE `pelanggan` (
  `id` int(11) NOT NULL,
  `serialNumber` varchar(25) NOT NULL,
  `nama` varchar(100) NOT NULL,
  `status` enum('Aktif','Tidak Aktif') NOT NULL DEFAULT 'Tidak Aktif',
  `selenoid` enum('ON','OFF') NOT NULL DEFAULT 'OFF',
  `createdAt` timestamp NOT NULL DEFAULT current_timestamp(),
  `updatedAt` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `pelanggan`
--

INSERT INTO `pelanggan` (`id`, `serialNumber`, `nama`, `status`, `selenoid`, `createdAt`, `updatedAt`) VALUES
(1, 'node-01', 'Pelanggan 1', 'Aktif', 'ON', '2024-09-09 03:31:35', '2024-10-15 01:37:35'),
(2, 'node-02', 'Pelanggan 2', 'Tidak Aktif', 'OFF', '2024-09-09 03:31:35', '2024-10-15 01:37:40'),
(4, 'node-03', 'Pelanggan 3', 'Tidak Aktif', 'OFF', '2024-10-15 02:03:45', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `sensor`
--

CREATE TABLE `sensor` (
  `id` int(11) NOT NULL,
  `serialNumber` varchar(25) NOT NULL,
  `ph` double NOT NULL,
  `ntu` double NOT NULL,
  `createdAt` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `sensor`
--

INSERT INTO `sensor` (`id`, `serialNumber`, `ph`, `ntu`, `createdAt`) VALUES
(1, 'node-01', 3, 11, '2024-09-23 06:22:44'),
(2, 'node-02', 3, 10, '2024-09-23 06:23:36');

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `nama` varchar(100) NOT NULL,
  `email` varchar(100) NOT NULL,
  `username` varchar(20) NOT NULL,
  `password` varchar(255) NOT NULL,
  `tahun` int(11) NOT NULL,
  `no_hp` varchar(14) NOT NULL,
  `createdAt` timestamp NOT NULL DEFAULT current_timestamp(),
  `updatedAt` timestamp NULL DEFAULT NULL ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `user`
--

INSERT INTO `user` (`id`, `nama`, `email`, `username`, `password`, `tahun`, `no_hp`, `createdAt`, `updatedAt`) VALUES
(1, 'Admin', 'admin@gmail.com', 'admin', '$2y$10$.pUHewzPqCdcnaOt5sUNf.FU6CTsEo.Rjki7jVeU7Bwb/cu74sN5K', 2024, '', '2024-03-13 07:13:19', '2024-04-19 01:34:44');

-- --------------------------------------------------------

--
-- Table structure for table `waterflow`
--

CREATE TABLE `waterflow` (
  `id` int(11) NOT NULL,
  `serialNumber` varchar(25) NOT NULL,
  `tanggal` date NOT NULL,
  `volume` double NOT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `waterflow`
--

INSERT INTO `waterflow` (`id`, `serialNumber`, `tanggal`, `volume`, `time`) VALUES
(1, 'node-01', '2024-09-23', 112, '2024-09-23 06:40:21');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `lokasi`
--
ALTER TABLE `lokasi`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `pelanggan`
--
ALTER TABLE `pelanggan`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `sensor`
--
ALTER TABLE `sensor`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `waterflow`
--
ALTER TABLE `waterflow`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `lokasi`
--
ALTER TABLE `lokasi`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `pelanggan`
--
ALTER TABLE `pelanggan`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `sensor`
--
ALTER TABLE `sensor`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `waterflow`
--
ALTER TABLE `waterflow`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
