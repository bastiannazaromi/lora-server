<?php
defined('BASEPATH') or exit('No direct script access allowed');

class M_Pelanggan extends CI_Model
{
	public function getAllPelanggan()
	{
		return $this->db->get('pelanggan')->result();
	}

	public function getOnePelanggan($id)
	{
		$this->db->where('id', $id);

		return $this->db->get('pelanggan')->row();
	}

	public function delete($id)
	{
		$this->db->where('id', $id);
		return $this->db->delete('pelanggan');
	}

	public function addPelanggan($data)
	{
		return $this->db->insert('pelanggan', $data);
	}

	public function editPelanggan($id, $data)
	{
		$this->db->where('id', $id);
		return $this->db->update('pelanggan', $data);
	}
}
