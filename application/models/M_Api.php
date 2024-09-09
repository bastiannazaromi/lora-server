<?php

defined('BASEPATH') or exit('No direct script access allowed');

class M_Api extends CI_Model
{

	public function cekPelanggan($where)
	{
		$this->db->where($where);

		return $this->db->get('pelanggan', 1)->row();
	}

	public function cekLokasi($where)
	{
		$this->db->where($where);

		return $this->db->get('lokasi', 1)->row();
	}
}

/* End of file M_Api.php */
