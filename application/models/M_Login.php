<?php

defined('BASEPATH') or exit('No direct script access allowed');

class M_Login extends CI_Model
{
	public function cekLogin($username, $password)
	{
		$this->db->where('username', $username);
		$data = $this->db->get('user')->row();

		if ($data) {
			if (password_verify($password, $data->password)) {
				$login        =    array(
					'is_logged_in' => true,
					'data'         => $data,
				);
				if ($login) {
					$this->session->set_userdata('user_login', $login);
					return 'sukses';
				}
			} else {
				return 'Password Salah';
			}
		} else {
			return 'Username tidak terdaftar';
		}
	}
}

  /* End of file M_Login.php */