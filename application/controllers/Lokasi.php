<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Lokasi extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		if (empty($this->session->userdata('user_login'))) {
			$this->session->set_flashdata('error', 'Anda belum login');

			redirect('login', 'resfresh');
		}
	}

	public function index()
	{
		$this->db->select('lokasi.*, pelanggan.nama');
		$this->db->join('pelanggan', 'pelanggan.serialNumber = lokasi.serialNumber', 'inner');

		$this->db->order_by('lokasi.serialNumber', 'asc');
		$lokasi = $this->db->get('lokasi')->result();

		$data = [
			'title'  => 'Lokasi',
			'page'   => 'lokasi/v_lokasi',
			'lokasi' => $lokasi
		];

		$this->load->view('index', $data);
	}
}

/* End of file Lokasi.php */
