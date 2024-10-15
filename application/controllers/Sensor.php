<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Sensor extends CI_Controller
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
		$this->db->select('sensor.*, pelanggan.nama');
		$this->db->join('pelanggan', 'pelanggan.serialNumber = sensor.serialNumber', 'inner');

		$this->db->order_by('sensor.id', 'desc');
		$sensor = $this->db->get('sensor')->result();

		$data = [
			'title'  => 'Sensor',
			'page'   => 'sensor/v_sensor',
			'sensor' => $sensor
		];

		$this->load->view('index', $data);
	}
}

/* End of file Sensor.php */
