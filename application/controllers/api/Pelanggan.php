<?php

defined('BASEPATH') or exit('No direct script access allowed');

class Pelanggan extends CI_Controller
{
	public function get()
	{
		$this->db->order_by('serialNumber', 'asc');
		$pelanggan = $this->db->get('pelanggan')->result();

		$res = [];

		if (count($pelanggan) > 0) {
			foreach ($pelanggan as $plg) {
				$res[] =
					$plg->serialNumber . '#' . $plg->status . '#' . $plg->selenoid . '#OK';
			}
		}

		echo implode('&', $res) . '&OK';
	}
}

/* End of file Pelanggan.php */
