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

		$pelanggan = $this->api->cekPelanggan(['serialNumber' => $serialNumber]);

		if (!$pelanggan) {
			echo 'Pelanggan tidak ditemukan!';
			exit;
		}

		if (($lat != 0 || $lat != null || $lat != '')) {
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
					'latitude'     => $lat,
					'longitude'    => $long
				];

				$update = $this->db->update('lokasi', $data);
				if ($update) {
					echo 'Berhasil menyimpan titik koordinat';
				} else {
					echo 'Server Error';
				}
			}
		} else {
			echo 'Latitude & Longitude tidak boleh kosong';
		}
	}
}

/* End of file Data.php */
