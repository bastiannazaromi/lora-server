<div class="content-wrapper">
	<!-- Content Header (Page header) -->
	<div class="content-header">
		<div class="container-fluid">
			<div class="row mb-2">
				<div class="col-sm-6">
					<h1 class="m-0"><?= $title; ?></h1>
				</div><!-- /.col -->
				<div class="col-sm-6">
					<ol class="breadcrumb float-sm-right">
						<li class="breadcrumb-item"><a href="#">Home</a></li>
						<li class="breadcrumb-item active">Dashboard</li>
					</ol>
				</div><!-- /.col -->
			</div><!-- /.row -->
		</div><!-- /.container-fluid -->
	</div>
	<!-- /.content-header -->

	<!-- Main content -->
	<section class="content">
		<div class="container-fluid">
			<?php if ($this->session->flashdata('sukses')) : ?>
				<div class="alert alert-success" role="alert">
					<?= $this->session->flashdata('sukses'); ?>
				</div>
			<?php endif; ?>

			<?php if ($this->session->flashdata('error')) : ?>
				<div class="alert alert-danger" role="alert">
					<?= $this->session->flashdata('error'); ?>
				</div>
			<?php endif; ?>

			<div class="row">
				<?php if (count($pelanggan) > 0) : ?>
					<?php foreach ($pelanggan as $i => $plg) : ?>
						<?php
						$this->db->where('serialNumber', $plg->serialNumber);

						$this->db->order_by('tanggal', 'desc');
						$waterflow = $this->db->get('waterflow', 1)->row();

						$this->db->where('serialNumber', $plg->serialNumber);

						$this->db->order_by('id', 'desc');
						$sensor = $this->db->get('sensor', 1)->row();

						$this->db->where('serialNumber', $plg->serialNumber);

						$lokasi = $this->db->get('lokasi')->row();
						?>
						<div class="col-xl-6">
							<div class="card">
								<div class="card-header bg-secondary">
									<h4><?= $plg->nama . ' - ' . $plg->serialNumber; ?></h4>
								</div>
								<div class="card-body">
									<div class="row">
										<?php if ($waterflow) : ?>
											<div class="col-lg-4 col-6">
												<div class="small-box bg-info">
													<div class="inner">
														<h3><?= $waterflow->volume; ?><sup style="font-size: 20px"> mL</sup></h3>

														<p>Volume</p>
													</div>
													<div class="icon">
														<i class="ion ion-bag"></i>
													</div>
												</div>
											</div>
										<?php endif; ?>
										<?php if ($sensor) : ?>
											<div class="col-lg-4 col-6">
												<div class="small-box bg-success">
													<div class="inner">
														<h3><?= $sensor->ph; ?></h3>

														<p>pH</p>
													</div>
													<div class="icon">
														<i class="ion ion-stats-bars"></i>
													</div>
												</div>
											</div>
											<div class="col-lg-4 col-6">
												<div class="small-box bg-warning">
													<div class="inner">
														<h3><?= $sensor->ntu; ?></h3>

														<p>NTU</p>
													</div>
													<div class="icon">
														<i class="ion ion-person-add"></i>
													</div>
												</div>
											</div>
										<?php endif; ?>
									</div>
									<?php if ($lokasi) : ?>
										<div class="row">
											<div class="col-lg-12">
												<div class="card">
													<div class="card-header">
														<h5>Lokasi</h5>
													</div>
													<div class="card-body">
														<iframe width="100%" height="450" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade" src="<?= "https://maps.google.com/maps?q=" . $lokasi->latitude . "," . $lokasi->longitude . "&amp;output=embed"; ?>">
														</iframe>
													</div>
												</div>
											</div>
										</div>
									<?php endif; ?>
								</div>
							</div>
						</div>
					<?php endforeach; ?>
				<?php endif; ?>
			</div>
		</div>
	</section>
</div>