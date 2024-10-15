<?php
defined('BASEPATH') or exit('No direct script access allowed');

class Pelanggan extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		if (empty($this->session->userdata('user_login'))) {
			$this->session->set_flashdata('error', 'Anda belum login');

			redirect('login', 'resfresh');
		}

		$this->load->model('M_Pelanggan', 'pelanggan');
	}

	public function index()
	{
		$pelanggan = $this->pelanggan->getAllPelanggan();

		$data = [
			'title'     => 'Halaman Pelanggan',
			'page'      => 'pelanggan/v_pelanggan',
			'pelanggan' => $pelanggan
		];

		$this->load->view('index', $data);
	}

	public function add()
	{
		$data = [
			'title' => 'Tambah Pelanggan',
			'page'  => 'pelanggan/v_addPelanggan'
		];

		$this->load->view('index', $data);
	}

	public function store()
	{
		$this->form_validation->set_rules('serialNumber', 'Serial Number', 'required');
		$this->form_validation->set_rules('nama', 'Nama', 'required');

		if ($this->form_validation->run() == FALSE) {
			$data = [
				'title' => 'Tambah User',
				'page'  => 'user/v_addUser'
			];

			$this->load->view('index', $data);
		} else {
			$serialNumber = $this->input->post('serialNumber');
			$nama         = $this->input->post('nama');

			$data = [
				'serialNumber' => $serialNumber,
				'nama'         => $nama,
			];

			$insert = $this->pelanggan->addPelanggan($data);

			if ($insert) {
				$this->session->set_flashdata('sukses', 'Data berhasil disimpan');

				redirect('pelanggan', 'refresh');
			} else {
				$this->session->set_flashdata('error', 'Data gagal disimpan!');

				redirect('pelanggan', 'refresh');
			}
		}
	}

	public function edit($id)
	{
		$pelanggan = $this->pelanggan->getOnePelanggan($id);

		$data = [
			'title'     => 'Edit Pelanggan',
			'page'      => 'pelanggan/v_editPelanggan',
			'pelanggan' => $pelanggan
		];

		$this->load->view('index', $data);
	}

	public function update()
	{
		$id = $this->input->post('id');

		$this->form_validation->set_rules('serialNumber', 'Serial Number', 'required');
		$this->form_validation->set_rules('nama', 'Nama', 'required');

		if ($this->form_validation->run() == false) {
			$this->edit($id);
		} else {
			$serialNumber = $this->input->post('serialNumber');
			$nama         = $this->input->post('nama');

			$data = [
				'serialNumber' => $serialNumber,
				'nama'         => $nama,
			];

			$update = $this->pelanggan->editPelanggan($id, $data);

			if ($update) {
				$this->session->set_flashdata('sukses', 'Data berhasil diedit');

				redirect('pelanggan', 'refresh');
			} else {
				$this->session->set_flashdata('error', 'Data gagak diedit');

				redirect('pelanggan', 'refresh');
			}
		}
	}

	public function delete($id)
	{
		$delete = $this->pelanggan->delete($id);

		if ($delete) {
			$this->session->set_flashdata('sukses', 'Data berhasil dihapus');

			redirect($_SERVER['HTTP_REFERER'], 'refresh');
		} else {
			$this->session->set_flashdata('error', 'Data gagal dihapus');

			redirect($_SERVER['HTTP_REFERER'], 'refresh');
		}
	}
}
