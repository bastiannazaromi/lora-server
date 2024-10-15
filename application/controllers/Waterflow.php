<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Waterflow extends CI_Controller
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
		$this->db->select('waterflow.*, pelanggan.nama');
		$this->db->join('pelanggan', 'pelanggan.serialNumber = waterflow.serialNumber', 'inner');

		$this->db->order_by('waterflow.tanggal', 'desc');
		$waterflow = $this->db->get('waterflow')->result();

		$data = [
			'title'     => 'Waterflow',
			'page'      => 'waterflow/v_waterflow',
			'waterflow' => $waterflow
		];

		$this->load->view('index', $data);
	}
}

/* End of file Waterflow.php */
