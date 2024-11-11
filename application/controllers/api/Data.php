<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Data extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();

		$this->load->model('M_Api', 'api');
	}

	public function post()
	{
		$serialNumber = $this->input->get('sn');
		$lat          = $this->input->get('lat');
		$long         = $this->input->get('long');

		$ph     = $this->input->get('ph');
		$ntu    = $this->input->get('ntu');
		$volume = $this->input->get('volume');

		$pelanggan = $this->api->cekPelanggan(['serialNumber' => $serialNumber]);

		if (!$pelanggan) {
			echo 'Pelanggan tidak ditemukan!';
			exit;
		}

		if (($lat != 0 && $lat != null && $lat != '' && $lat != '0') && ($long != 0 && $long != null && $long != '' && $long != '0')) {
			$cek = $this->api->cekLokasi(['serialNumber' => $serialNumber]);

			if (!$cek) {
				$data = [
					'serialNumber' => $serialNumber,
					'latitude'     => $lat,
					'longitude'    => $long
				];

				$insert = $this->db->insert('lokasi', $data);
				if ($insert) {
					echo 'Berhasil menyimpan titik koordinat';
				} else {
					echo 'Server Error';
				}
			} else {
				$data = [
					'latitude'  => $lat,
					'longitude' => $long
				];

				$update = $this->db->update('lokasi', $data);
				if ($update) {
					echo 'Berhasil menyimpan titik koordinat';
				} else {
					echo 'Server Error';
				}
			}
		} else if (($ph != null || $ph != 0) && ($ntu != null || $ntu != '')) {
			$this->db->order_by('id', 'desc');
			$cek = $this->db->get('sensor', 1)->row();

			if ($cek) {
				if (($ph != $cek->ph) || ($ntu != $cek->ntu)) {

					$data = [
						'serialNumber' => $serialNumber,
						'ph'           => (float) $ph,
						'ntu'          => (float) $ntu
					];

					$insert = $this->db->insert('sensor', $data);
					if ($insert) {
						echo 'Berhasil menyimpan nilai ph dan ntu';
					} else {
						echo 'Server Error';
					}
				} else {
					echo 'Berhasil, nilai masih sama';
				}
			} else {
				$data = [
					'serialNumber' => $serialNumber,
					'ph'           => (float) $ph,
					'ntu'          => (float) $ntu
				];

				$insert = $this->db->insert('sensor', $data);
				if ($insert) {
					echo 'Berhasil menyimpan nilai ph dan ntu';
				} else {
					echo 'Server Error';
				}
			}
		} else if ($volume != null) {
			$date = date('Y-m-d');
			$this->db->where('tanggal', $date);
			$this->db->order_by('tanggal', 'desc');
			$cek = $this->db->get('waterflow', 1)->row();

			if (!$cek) {
				$data = [
					'serialNumber' => $serialNumber,
					'tanggal'      => $date,
					'volume'       => (float) $volume,
				];

				$insert = $this->db->insert('waterflow', $data);
				if ($insert) {
					echo 'Berhasil menyimpan nilai volume';
				} else {
					echo 'Server Error';
				}
			} else {
				$data = [
					'volume' => (float) $volume,
				];

				$this->db->where('id', $cek->id);
				$update = $this->db->update('waterflow', $data);
				if ($update) {
					echo 'Berhasil menyimpan nilai volume';
				} else {
					echo 'Server Error';
				}
			}
		} else {
			echo 'Isi parameter dengan benar!';
		}
	}
}

  /* End of file Data.php */
